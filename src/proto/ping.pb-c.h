/* Generated by the protocol buffer compiler.  DO NOT EDIT! */
/* Generated from: ping.proto */

#ifndef PROTOBUF_C_ping_2eproto__INCLUDED
#define PROTOBUF_C_ping_2eproto__INCLUDED

#include <protobuf-c/protobuf-c.h>

PROTOBUF_C__BEGIN_DECLS

#if PROTOBUF_C_VERSION_NUMBER < 1000000
# error This file was generated by a newer version of protoc-c which is incompatible with your libprotobuf-c headers. Please update your headers.
#elif 1003000 < PROTOBUF_C_MIN_COMPILER_VERSION
# error This file was generated by an older version of protoc-c which is incompatible with your libprotobuf-c headers. Please regenerate this file with a newer version of protoc-c.
#endif


typedef struct _Ping__PingReq Ping__PingReq;
typedef struct _Ping__PingRsp Ping__PingRsp;


/* --- enums --- */


/* --- messages --- */

/*
 * service ping
 */
struct  _Ping__PingReq
{
  ProtobufCMessage base;
  uint32_t sno;
};
#define PING__PING_REQ__INIT \
 { PROTOBUF_C_MESSAGE_INIT (&ping__ping_req__descriptor) \
    , 0 }


struct  _Ping__PingRsp
{
  ProtobufCMessage base;
  uint32_t sno;
};
#define PING__PING_RSP__INIT \
 { PROTOBUF_C_MESSAGE_INIT (&ping__ping_rsp__descriptor) \
    , 0 }


/* Ping__PingReq methods */
void   ping__ping_req__init
                     (Ping__PingReq         *message);
size_t ping__ping_req__get_packed_size
                     (const Ping__PingReq   *message);
size_t ping__ping_req__pack
                     (const Ping__PingReq   *message,
                      uint8_t             *out);
size_t ping__ping_req__pack_to_buffer
                     (const Ping__PingReq   *message,
                      ProtobufCBuffer     *buffer);
Ping__PingReq *
       ping__ping_req__unpack
                     (ProtobufCAllocator  *allocator,
                      size_t               len,
                      const uint8_t       *data);
void   ping__ping_req__free_unpacked
                     (Ping__PingReq *message,
                      ProtobufCAllocator *allocator);
/* Ping__PingRsp methods */
void   ping__ping_rsp__init
                     (Ping__PingRsp         *message);
size_t ping__ping_rsp__get_packed_size
                     (const Ping__PingRsp   *message);
size_t ping__ping_rsp__pack
                     (const Ping__PingRsp   *message,
                      uint8_t             *out);
size_t ping__ping_rsp__pack_to_buffer
                     (const Ping__PingRsp   *message,
                      ProtobufCBuffer     *buffer);
Ping__PingRsp *
       ping__ping_rsp__unpack
                     (ProtobufCAllocator  *allocator,
                      size_t               len,
                      const uint8_t       *data);
void   ping__ping_rsp__free_unpacked
                     (Ping__PingRsp *message,
                      ProtobufCAllocator *allocator);
/* --- per-message closures --- */

typedef void (*Ping__PingReq_Closure)
                 (const Ping__PingReq *message,
                  void *closure_data);
typedef void (*Ping__PingRsp_Closure)
                 (const Ping__PingRsp *message,
                  void *closure_data);

/* --- services --- */


/* --- descriptors --- */

extern const ProtobufCMessageDescriptor ping__ping_req__descriptor;
extern const ProtobufCMessageDescriptor ping__ping_rsp__descriptor;

PROTOBUF_C__END_DECLS


#endif  /* PROTOBUF_C_ping_2eproto__INCLUDED */