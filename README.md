# os_timestamps
### [https://github.com/quoscient/os_timestamps](https://github.com/quoscient/os_timestamps)
Profile how your Unix-like OS (Linux, BSD...)  modifies MACB timestamps.

# Build
- `mkdir build; cd build/`
- `cmake ../src/`
- `make`

# OS Profiling
Please read the blog post for more information on the results and OS implementations:
- https://medium.com/@quoscient/mac-b-timestamps-across-posix-implementations-linux-openbsd-freebsd-1e2d5893e4f

`profile_os` profiles common file operations:
- New file/dir
- File read (cat)
- File write (echo AA > file)
- File change (chown, chmod)
- Local File Copy
- Volume file copy
- ...

To have complete results you will need:
- For "Volume Copy" (copy a file to a different file system): the other file system to test, mounted into mnt/
    - On Linux you can create an Ext4 fs with:
        - `dd if=/dev/zero of=profileos.ext4 bs=4M count=10`
        - `mkfs.ext4 -O extra_isize -I 256 profileos.ext4`
    - On OpenBSD you may use /tmp/ as it is typically on a separate partition
- For reliable chown tests ("File Change"...): the GID of a group you can attribute to a file (you shall be able to do `chown :GID file`)
    - On OpenBSD & FreeBSD, GID 0 (wheel) can be used

```
$ ./profile_os -m mnt/ -g 1002
INFO: Directory for tests is: tmp_os_profile_10/
INFO: Volume path is: mnt/tmp_os_profile_10/
File Creation (PROFILE.OS.FILE.NEW):
dir/
  M.C.
newfile
  MACB
File Copy (new) (PROFILE.OS.FILE.COPY.NEW):
src
  .A..
srcdir/
  ....
dst
  MACB
dstdir/
  M.C.
  [...]
```

Symbols you will encounter:
```
M/A/C/B - M/A/C/B was updated to current time
m/a/c/b - M/A/C/B was set to the same m/a/c/b value as the source file/dir
>       - M/A/C/B was set to the same value as the source file/dir (same as m/a/c/b but for the same timestamp)
.       - M/A/C/B was not modified
d       - M/A/C/B seem to have been updated after the command ended (delay - this may hidden with -d option)
0       - M/A/C/B was 0 after command execution
!       - Error (mostly: the file did not exist anymore)
```

Two CSV files are also created:
- os_profile_results.csv - Contains a condensed view of the results, can be used for comparison
- os_profile_flags.csv   - Contains a more detailed view of the results, explaining precisely what happens to each timestamp

The provided results will need some interpretation and a bit of context (mount options, OS configuration...) to be fully understood.

## POSIX
POSIX specifies MAC updates, the manually generated os_profile_results.csv is here:
- [results/2019_11_29/profileos/posix/os_profile.csv](results/2019_11_29/profileos/posix/os_profile.csv)

`*` is an additional symbol for when POSIX leaves choice to the implementation:
> Some implementations mark for update the last file status change timestamp of renamed files and some do not.

## Results
For easier comparison of MAC updates, a result file without B updates (os_profile_results_mac.csv) is provided.

Some more PDF options are in the Release section.

Linux:
- PDF: [linux_macb.pdf](https://github.com/quoscient/os_timestamps/releases/download/2019_12_03/linux_macb.pdf)
- os_profile_results.csv: [results/2019_11_29/profileos/linux/os_profile_results.csv](results/2019_11_29/profileos/linux/os_profile_results.csv)
- os_profile_results_mac.csv: [results/2019_11_29/profileos/linux/os_profile_results_mac.csv](results/2019_11_29/profileos/linux/os_profile_results_mac.csv)
- os_profile_flags.csv: [results/2019_11_29/profileos/linux/os_profile_flags.csv](results/2019_11_29/profileos/linux/os_profile_flags.csv)

OpenBSD:
- PDF: [openbsd_mac.pdf](https://github.com/quoscient/os_timestamps/releases/download/2019_12_03/openbsd_mac.pdf)
- os_profile_results.csv: [results/2019_11_29/profileos/openbsd/os_profile_results.csv](results/2019_11_29/profileos/openbsd/os_profile_results.csv)
- os_profile_results_mac.csv: [results/2019_11_29/profileos/openbsd/os_profile_results_mac.csv](results/2019_11_29/profileos/openbsd/os_profile_results_mac.csv)
- os_profile_flags.csv: [results/2019_11_29/profileos/openbsd/os_profile_flags.csv](results/2019_11_29/profileos/openbsd/os_profile_flags.csv)

FreeBSD:
- PDF: [freebsd_macb.pdf](https://github.com/quoscient/os_timestamps/releases/download/2019_12_03/freebsd_macb.pdf)
- os_profile_results.csv: [results/2019_11_29/profileos/freebsd/os_profile_results.csv](results/2019_11_29/profileos/freebsd/os_profile_results.csv)
- os_profile_results_mac.csv: [results/2019_11_29/profileos/freebsd/os_profile_results_mac.csv](results/2019_11_29/profileos/freebsd/os_profile_results_mac.csv)
- os_profile_flags.csv: [results/2019_11_29/profileos/freebsd/os_profile_flags.csv](results/2019_11_29/profileos/freebsd/os_profile_flags.csv)

# Command Profiling
`profile_cmd` is an interactive tool to profile any shell commands.

You need to manually define which files/dirs you want to watch for change.

The first watched path is considered as the source file/dir (for **>** and **m/a/c/b**), so be careful to define it correctly.

**New File (Linux):**
```
$ ./profile_cmd -w file 'touch file'
file
  MACB
```

**Dir Move (Linux):**
```
$ ./profile_cmd -w dir/ -w dst/ -w dst/dir/ 'mv dir/ dst/'
dir/
  !!!!
dst/
  M.C.
dst/dir/
  >>C>
```

# Tools
## Get MACB Timestamps

`prototype_file_ts TARGET [MODE]` will output the target's MACB timestamps.

As it uses the **stat()** (and **statx()** on Linux) system calls, it shall not update any timestamp of the target.

Mode can be:
- 0 (default): uses **stat** to get MAC and gets B by OS-specific method that follows symlinks
- 1: same as 0 but with **lstat** and OS-specific method that does not follow symlinks
- 2: Linux only: gets MACB with **statx**, with the AT_SYMLINK_NOFOLLOW flag (do not follow symlinks)

Implemented OS-specific methods to read B:
- Linux: use the **statx** system call
- OpenBSD: **struct stat** has a field called **__st_birthtim**
- FreeBSD: **struct stat** has a field called **st_birthtim**

```
$ ./prototype_file_ts file
file: (stat + B)
M: Mon Dec  2 13:59:38 2019 - ns: 766478455
A: Mon Dec  2 13:59:36 2019 - ns: 870434493
C: Mon Dec  2 13:59:42 2019 - ns: 554566399
B: Mon Dec  2 13:42:32 2019 - ns: 126956318
```
