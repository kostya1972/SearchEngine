<h1>SearchEngine</h1>  

<h2>Short description</h2>  
It is an engine for files search,are pointed in file <b>config.json</b>(the files located in project directory <b>Resources</b>,   
but you may to write your own files paths), by requests strings, that are pointed in file <b>request.json</b>.

<h2>Technology and languages</h2>  
The project written via <b>C++(v.14)</b>, by <b>IDE CLion2020.3.3</b>, with using <b>json</b> technology.  
For testing is uses framework <b>Google Test</b>.  

<h2>Install</h2>  
You need to clone and build this repository, by<br>  
git clone https://github.com/kostya1972/SearchEngine<br>
cd SearchEngine<br>
cmake -G "CodeBlocks - MinGW Makefiles" SearchEngine<br>
cmake --build SearchEngine -- -j 3<br>
 
<h2>Running</h2>  
The program is run by command <i>SearchEngine</i>.  
Before running you should to write files paths for searching, to section <b>files</b> of file <b>config.json</b>,  
since the engine read files paths, while program launching.  
You may write words for search as well before, as after program launching, to file <b>request.json</b>.  
For search you need input command <i>request</i>.  
Results of the search are put to file <b>answers.json</b>, in order of relevant, are pointed in a field <b>rank</b>.    
<b>SearchEngine</b> updates a search base in term, pointing in a field <b>update_time</b> of file <b>config.json</b>.   
<b>SearchEngine</b> is closed by command <i>quit</i>.  

<h2>Testing</h2>  
For to launch testing, you need uncomment these strings at main.cpp:  

```C++  
//#include "tests.h"  

int main() {  

    //testing::InitGoogleTest();  
    //return RUN_ALL_TESTS();  
```  
and this strings at MakeLists.txt:  
```CMakeLists.txt  
     #add_subdirectory(googletest-1.16.0)  
     #include_directories(${gtest_SOURCE_DIR}/include ${gtest_SOURCE_DIR})  
     #set(gtest_disable_pthreads on)  
     #set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)  

     #target_link_libraries(SearchEngine gtest gtest_main)  
     #enable_testing()  
```  

<h2>Dependence</h2>  
1. SearchEngine uses library <b>nlohmann_json</b>, for work with <b>json</b> format files.  
2. For testing <b>SearchEngine</b> uses <b>google test(v.1.16.0)</b>.  

   This libraries located in project directories: nlohmann_json, googletest-1.16.0.  
   Files <b>json</b> located in project directory <b>Json_files</b>.  
   
