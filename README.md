# Installation (Ubuntu/Debian)
```
sudo apt update
sudo apt install build-essential cmake libssl-dev
```
# Build
```
mkdir build
cd build
cmake ..
cmake --build .
```
# Usage
```
./scanner_exe/scanner --base malware_db.csv --log scan_report.log --path /path/to/scan
```
# Test running
```
cd build
./tests/tests
```
# Output example
```
./scanner --base ../../test_dir/hashs.scv --log report.log --path ../../test_dir

[2025-09-17 22:03:39] File: ../../test_dir/test4/test5, Hash: 073faa4ba9f76be669c019233f4804b0, Verdict: Clean
[2025-09-17 22:03:39] File: ../../test_dir/test2.txt, Hash: 2194628c058667579e0a871e7202a397, Verdict: Clean
[2025-09-17 22:03:39] File: ../../test_dir/test3.txt, Hash: 1864d6c1b9c359ca3dad3770a7caaf5f, Verdict: Clean
[2025-09-17 22:03:39] File: ../../test_dir/test1.txt, Hash: e6a3bd414bf3dff0487ffc046101a421, Verdict: Clean
[2025-09-17 22:03:39] File: ../../test_dir/hashs.scv, Hash: fa49534b81536d1eccfd25f29e3fd2c7, Verdict: Clean
Scan completed in 1 ms
Processed 5 files
```
