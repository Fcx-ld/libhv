# 命令将一系列的测试程序和应用程序添加到EXAMPLES列表中。这些程序将作为构建目标被编译。
list(APPEND STUDY
    StudyTest
)
if(WITH_EVPP)
    include_directories(../cpputil ../evpp)

    # StudyTest
    add_executable(StudyTest StudyTest.cpp)
    target_link_libraries(StudyTest ${HV_LIBRARIES})
endif()

include_directories(.. ../base ../ssl ../event ../util)
# add_executable(StudyTest StudyTest.cpp)
# target_link_libraries(StudyTest ${HV_LIBRARIES})

# 使用add_custom_target(...)命令创建一个自定义目标examples，它依赖于EXAMPLES列表中的所有可执行文件。这允许用户通过构建examples目标来编译所有的示例程序。
add_custom_target(study DEPENDS ${STUDY})
