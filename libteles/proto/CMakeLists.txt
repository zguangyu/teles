include(FindProtobuf)
find_package(Protobuf REQUIRED)

file(GLOB PROTO_FILES "*.proto")
PROTOBUF_GENERATE_CPP(PROTO_SRC PROTO_HEADER ${PROTO_FILES})

add_library(proto ${PROTO_HEADER} ${PROTO_SRC})
target_link_libraries(proto
    PUBLIC protobuf::libprotobuf
)
