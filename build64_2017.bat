set project=shape

rem //expected build dir structure
set buildexp=build\\VS15_2017_x64

set currentdir=%cd%
set builddir=.\\%buildexp%
set deploydir=.\\deploy\\VS15_2017_x64

mkdir %builddir%

rem //get absolute path to to deploydir
mkdir %deploydir%
pushd %deploydir%
set deploydir=%cd%
popd

rem //launch cmake to generate build environment
pushd %builddir%
cmake -G "Visual Studio 15 2017 Win64" -DBUILD_TESTING:BOOL=true -DSHAPE_DEPLOY:PATH=%deploydir% %currentdir% 
popd

rem //build from generated build environment
cmake --build %builddir%
