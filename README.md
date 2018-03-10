# clog
A simple yet fast and bare minimal asynchronous logging library for C

#### CMake

#### Header Only
Use the given amalgamate.py for combining the source and header files and copy the result `clog.h` into your project,
`amalgamate.py include/clog/clog.h

### Usage
#### Logging can be done by,
Include the header file, `#include<clog/clog.h>`
<pre><code>
const char *TAG = "TEST";  
log_inf(TAG, "%s", "test"); //log info  
log_err(TAG, "%s", "test"); //log error  
log_per(TAG, "%s", "test"); //similar to error but calls perror  
log_fat(TAG, "%s", "test"); //log fatal errors
</code></pre>

#### During production
Just flip the switch to disable from your CMakeLists.txt of your project
`cmake -DCLOG_ENABLE=false`