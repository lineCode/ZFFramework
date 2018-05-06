# 0.1.4 (20180506)

* change to use shell script to create project files
* optimize file structure for impl projects

# 0.1.3 (20180503)

* redesign of skin and lang logic
* optimize path info impl
* add `ZFPROPERTY_WEAK`
* allow reset property value by reflect
* fix lua operators impl
* optimize observer performance
* trim output executable size
* optimize build script

# 0.1.2 (20180423)

* update build script
* optimize identity logic
* optimize timeline animation impl
* add property animation
* optimize event dispatch
* optimize lua api
* bugfix: native view cache doesn't properly reset state

# 0.1.1 (20180319)

* upgrade Android project files
* update CI script
* impl lib's header can now be exported
* optimize external ZFModule build logic and script
* optimize method and property declaration
* reduce recursive header include to improve compile time
* trim output executable size

# 0.1.0 (20180218)

* first commit
* main platforms and issues/TODOs
    * ZFProjCreator
        * convenient only for creating new projects,
          no future plan on modifying or migrating existing projects
    * iOS
        * Cocoapods : should work perfectly now, one step to use, no annoying config
        * XCode : work but still annoying to setup C++ library envs
    * Android
        * Android Studio : work but still annoying to setup C++ library envs
    * Qt
        * QtCreator : work but still annoying to setup C++ library envs

