cmake_minimum_required(VERSION 4.0)

project(EffectiveCpp)

add_subdirectory(Item28)    #避免返回handles指向对象内部的成分

add_subdirectory(Item30)    #透彻了解inlining的里里外外