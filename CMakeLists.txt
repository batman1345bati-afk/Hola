cmake_minimum_required(VERSION 3.25)
project(NexusHelperPro)

find_package(Geode REQUIRED)

geode_target(NexusHelperPro)

target_sources(NexusHelperPro PRIVATE
    main.cpp
)
