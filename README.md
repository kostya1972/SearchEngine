#SearchEngine

##Short description
It is an engine for files searching,are pointed in file 'config.json'(the files located in project directory 'Resources', 
but you may to write your own files paths), by requests strings,that are pointed in file 'request.json'.

##Technology and languages
The project written via C++(v.14), by IDE CLion2020.3.3, with using 'json' technology.
For testing is uses framework 'Google Test'.

##Install
You need just to clone this repository, by 
``` git clone https://github.com/kostya1972/```.
 
##Running
The program is run by command 'SearchEngine'.
Before running you should to write files paths for searching, to section 'files' of file 'config.json',
since the engine read files paths, while program launching.
You may write words for searching as well before, as after launching program, to file 'request.json'.
For searching you need input command 'request'.
Results of the searching are put to file 'answers.json' in order of relevant, are pointed in a field 'rank'.
SearchEngine is closed by command 'quit'.

##Testing
For to launch testing, you need uncomment this string at main.cpp:
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

##Dependence
1. SearchEngine uses library 'nlohmann_json', for work with 'json' format files.
2. For testing SearchEngine uses google test(v.1.16.0).

   This libraries located in project directories: nlohmann_json, googletest-1.16.0.
   Files 'json' located in project directory 'Json_files'.
   
