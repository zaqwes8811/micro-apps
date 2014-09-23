# http://stackoverflow.com/questions/2580934/how-to-specify-different-debug-release-output-directories-in-qmake-pro-file

TARGET = a.app
TEMPLATE = app
CONFIG += console
CONFIG -= app_bundle
CONFIG -= qt
OBJECTS_DIR = obj

BOOST=/home/zaqwes/work/third_party/boost_1_55_0
LOCAL_3RDPARTY=../3rdparty

# -std=c++11 -pg
QMAKE_CXXFLAGS += -O2  -Wall -Wno-deprecated
# -std=c++11

SOURCES += \
    ./src/pq_dal_test.cc \  
    ./src/storage_access.cc \
    ./src/model_test.cc \
    $$LOCAL_3RDPARTY/gmock-1.6.0/fused-src/gmock-gtest-all.cc \
    launcher.cc

INCLUDEPATH += \
  $$BOOST \
  $$LOCAL_3RDPARTY/loki-0.1.7/include \
  $$LOCAL_3RDPARTY/gmock-1.6.0/fused-src/ \
  src \
  /home/zaqwes/work/research_toolboxes/in-the-vicinity-cc/toolbox
  
# -ltbb
LIBS += -lpthread -lrt  -lpq -lpqxx




