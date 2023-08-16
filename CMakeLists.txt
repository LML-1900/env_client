cmake_minimum_required(VERSION 3.25)
project(env_client)
set(CMAKE_CXX_STANDARD 17)

# Locate protobuf installation
set(protobuf_MODULE_COMPATIBLE TRUE)
list(APPEND CMAKE_PREFIX_PATH "$ENV{HOME}/.local")
find_package(Protobuf CONFIG REQUIRED)
message(STATUS "Using protobuf ${Protobuf_VERSION}")

# Locate gRPC installation
list(APPEND CMAKE_PREFIX_PATH "$ENV{HOME}/.local") # replace with your actual path
find_package(gRPC CONFIG REQUIRED)

find_package(Poco REQUIRED Foundation Net Util)

# Proto file
get_filename_component(hw_proto "./env_service.proto" ABSOLUTE)
get_filename_component(hw_proto_path "${hw_proto}" PATH)

# Generated sources
set(hw_proto_srcs "${CMAKE_CURRENT_SOURCE_DIR}/env_service.pb.cc")
set(hw_proto_hdrs "${CMAKE_CURRENT_SOURCE_DIR}/env_service.pb.h")
set(hw_grpc_srcs "${CMAKE_CURRENT_SOURCE_DIR}/env_service.grpc.pb.cc")
set(hw_grpc_hdrs "${CMAKE_CURRENT_SOURCE_DIR}/env_service.grpc.pb.h")

add_custom_command(
        OUTPUT "${hw_proto_srcs}" "${hw_proto_hdrs}" "${hw_grpc_srcs}" "${hw_grpc_hdrs}"
        COMMAND protoc
        ARGS --grpc_out "${CMAKE_CURRENT_SOURCE_DIR}"
        --cpp_out "${CMAKE_CURRENT_SOURCE_DIR}"
        -I "${hw_proto_path}"
        --plugin=protoc-gen-grpc=$ENV{HOME}/.local/bin/grpc_cpp_plugin #replace with your actual path
        "${hw_proto}"
        DEPENDS "${hw_proto}")

add_executable(env_client main.cpp ${hw_proto_srcs} ${hw_grpc_srcs} EnvMidWare.cpp EnvMidWare.h Dem.h)

target_link_libraries(env_client
        gRPC::grpc++
        gRPC::grpc++_reflection
        protobuf::libprotobuf)
target_link_libraries(env_client Poco::Foundation Poco::Net Poco::Util)