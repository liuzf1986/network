// Generated by the protocol buffer compiler.  DO NOT EDIT!
// source: common.proto

#define INTERNAL_SUPPRESS_PROTOBUF_FIELD_DEPRECATION
#include "common.pb.h"

#include <algorithm>

#include <google/protobuf/stubs/common.h>
#include <google/protobuf/stubs/once.h>
#include <google/protobuf/io/coded_stream.h>
#include <google/protobuf/wire_format_lite_inl.h>
#include <google/protobuf/descriptor.h>
#include <google/protobuf/generated_message_reflection.h>
#include <google/protobuf/reflection_ops.h>
#include <google/protobuf/wire_format.h>
// @@protoc_insertion_point(includes)

namespace account {

namespace {

const ::google::protobuf::Descriptor* BaseReq_descriptor_ = NULL;
const ::google::protobuf::internal::GeneratedMessageReflection*
  BaseReq_reflection_ = NULL;
const ::google::protobuf::Descriptor* PingRequest_descriptor_ = NULL;
const ::google::protobuf::internal::GeneratedMessageReflection*
  PingRequest_reflection_ = NULL;
const ::google::protobuf::Descriptor* PongResponse_descriptor_ = NULL;
const ::google::protobuf::internal::GeneratedMessageReflection*
  PongResponse_reflection_ = NULL;

}  // namespace


void protobuf_AssignDesc_common_2eproto() {
  protobuf_AddDesc_common_2eproto();
  const ::google::protobuf::FileDescriptor* file =
    ::google::protobuf::DescriptorPool::generated_pool()->FindFileByName(
      "common.proto");
  GOOGLE_CHECK(file != NULL);
  BaseReq_descriptor_ = file->message_type(0);
  static const int BaseReq_offsets_[2] = {
    GOOGLE_PROTOBUF_GENERATED_MESSAGE_FIELD_OFFSET(BaseReq, seskey_),
    GOOGLE_PROTOBUF_GENERATED_MESSAGE_FIELD_OFFSET(BaseReq, accid_),
  };
  BaseReq_reflection_ =
    new ::google::protobuf::internal::GeneratedMessageReflection(
      BaseReq_descriptor_,
      BaseReq::default_instance_,
      BaseReq_offsets_,
      GOOGLE_PROTOBUF_GENERATED_MESSAGE_FIELD_OFFSET(BaseReq, _has_bits_[0]),
      GOOGLE_PROTOBUF_GENERATED_MESSAGE_FIELD_OFFSET(BaseReq, _unknown_fields_),
      -1,
      ::google::protobuf::DescriptorPool::generated_pool(),
      ::google::protobuf::MessageFactory::generated_factory(),
      sizeof(BaseReq));
  PingRequest_descriptor_ = file->message_type(1);
  static const int PingRequest_offsets_[1] = {
    GOOGLE_PROTOBUF_GENERATED_MESSAGE_FIELD_OFFSET(PingRequest, basereq_),
  };
  PingRequest_reflection_ =
    new ::google::protobuf::internal::GeneratedMessageReflection(
      PingRequest_descriptor_,
      PingRequest::default_instance_,
      PingRequest_offsets_,
      GOOGLE_PROTOBUF_GENERATED_MESSAGE_FIELD_OFFSET(PingRequest, _has_bits_[0]),
      GOOGLE_PROTOBUF_GENERATED_MESSAGE_FIELD_OFFSET(PingRequest, _unknown_fields_),
      -1,
      ::google::protobuf::DescriptorPool::generated_pool(),
      ::google::protobuf::MessageFactory::generated_factory(),
      sizeof(PingRequest));
  PongResponse_descriptor_ = file->message_type(2);
  static const int PongResponse_offsets_[1] = {
  };
  PongResponse_reflection_ =
    new ::google::protobuf::internal::GeneratedMessageReflection(
      PongResponse_descriptor_,
      PongResponse::default_instance_,
      PongResponse_offsets_,
      GOOGLE_PROTOBUF_GENERATED_MESSAGE_FIELD_OFFSET(PongResponse, _has_bits_[0]),
      GOOGLE_PROTOBUF_GENERATED_MESSAGE_FIELD_OFFSET(PongResponse, _unknown_fields_),
      -1,
      ::google::protobuf::DescriptorPool::generated_pool(),
      ::google::protobuf::MessageFactory::generated_factory(),
      sizeof(PongResponse));
}

namespace {

GOOGLE_PROTOBUF_DECLARE_ONCE(protobuf_AssignDescriptors_once_);
inline void protobuf_AssignDescriptorsOnce() {
  ::google::protobuf::GoogleOnceInit(&protobuf_AssignDescriptors_once_,
                 &protobuf_AssignDesc_common_2eproto);
}

void protobuf_RegisterTypes(const ::std::string&) {
  protobuf_AssignDescriptorsOnce();
  ::google::protobuf::MessageFactory::InternalRegisterGeneratedMessage(
    BaseReq_descriptor_, &BaseReq::default_instance());
  ::google::protobuf::MessageFactory::InternalRegisterGeneratedMessage(
    PingRequest_descriptor_, &PingRequest::default_instance());
  ::google::protobuf::MessageFactory::InternalRegisterGeneratedMessage(
    PongResponse_descriptor_, &PongResponse::default_instance());
}

}  // namespace

void protobuf_ShutdownFile_common_2eproto() {
  delete BaseReq::default_instance_;
  delete BaseReq_reflection_;
  delete PingRequest::default_instance_;
  delete PingRequest_reflection_;
  delete PongResponse::default_instance_;
  delete PongResponse_reflection_;
}

void protobuf_AddDesc_common_2eproto() {
  static bool already_here = false;
  if (already_here) return;
  already_here = true;
  GOOGLE_PROTOBUF_VERIFY_VERSION;

  ::google::protobuf::DescriptorPool::InternalAddGeneratedFile(
    "\n\014common.proto\022\007account\"(\n\007BaseReq\022\016\n\006se"
    "sKey\030\001 \002(\014\022\r\n\005accId\030\002 \002(\r\"0\n\013PingRequest"
    "\022!\n\007baseReq\030\001 \002(\0132\020.account.BaseReq\"\016\n\014P"
    "ongResponse", 131);
  ::google::protobuf::MessageFactory::InternalRegisterGeneratedFile(
    "common.proto", &protobuf_RegisterTypes);
  BaseReq::default_instance_ = new BaseReq();
  PingRequest::default_instance_ = new PingRequest();
  PongResponse::default_instance_ = new PongResponse();
  BaseReq::default_instance_->InitAsDefaultInstance();
  PingRequest::default_instance_->InitAsDefaultInstance();
  PongResponse::default_instance_->InitAsDefaultInstance();
  ::google::protobuf::internal::OnShutdown(&protobuf_ShutdownFile_common_2eproto);
}

// Force AddDescriptors() to be called at static initialization time.
struct StaticDescriptorInitializer_common_2eproto {
  StaticDescriptorInitializer_common_2eproto() {
    protobuf_AddDesc_common_2eproto();
  }
} static_descriptor_initializer_common_2eproto_;

// ===================================================================

#ifndef _MSC_VER
const int BaseReq::kSesKeyFieldNumber;
const int BaseReq::kAccIdFieldNumber;
#endif  // !_MSC_VER

BaseReq::BaseReq()
  : ::google::protobuf::Message() {
  SharedCtor();
  // @@protoc_insertion_point(constructor:account.BaseReq)
}

void BaseReq::InitAsDefaultInstance() {
}

BaseReq::BaseReq(const BaseReq& from)
  : ::google::protobuf::Message() {
  SharedCtor();
  MergeFrom(from);
  // @@protoc_insertion_point(copy_constructor:account.BaseReq)
}

void BaseReq::SharedCtor() {
  ::google::protobuf::internal::GetEmptyString();
  _cached_size_ = 0;
  seskey_ = const_cast< ::std::string*>(&::google::protobuf::internal::GetEmptyStringAlreadyInited());
  accid_ = 0u;
  ::memset(_has_bits_, 0, sizeof(_has_bits_));
}

BaseReq::~BaseReq() {
  // @@protoc_insertion_point(destructor:account.BaseReq)
  SharedDtor();
}

void BaseReq::SharedDtor() {
  if (seskey_ != &::google::protobuf::internal::GetEmptyStringAlreadyInited()) {
    delete seskey_;
  }
  if (this != default_instance_) {
  }
}

void BaseReq::SetCachedSize(int size) const {
  GOOGLE_SAFE_CONCURRENT_WRITES_BEGIN();
  _cached_size_ = size;
  GOOGLE_SAFE_CONCURRENT_WRITES_END();
}
const ::google::protobuf::Descriptor* BaseReq::descriptor() {
  protobuf_AssignDescriptorsOnce();
  return BaseReq_descriptor_;
}

const BaseReq& BaseReq::default_instance() {
  if (default_instance_ == NULL) protobuf_AddDesc_common_2eproto();
  return *default_instance_;
}

BaseReq* BaseReq::default_instance_ = NULL;

BaseReq* BaseReq::New() const {
  return new BaseReq;
}

void BaseReq::Clear() {
  if (_has_bits_[0 / 32] & 3) {
    if (has_seskey()) {
      if (seskey_ != &::google::protobuf::internal::GetEmptyStringAlreadyInited()) {
        seskey_->clear();
      }
    }
    accid_ = 0u;
  }
  ::memset(_has_bits_, 0, sizeof(_has_bits_));
  mutable_unknown_fields()->Clear();
}

bool BaseReq::MergePartialFromCodedStream(
    ::google::protobuf::io::CodedInputStream* input) {
#define DO_(EXPRESSION) if (!(EXPRESSION)) goto failure
  ::google::protobuf::uint32 tag;
  // @@protoc_insertion_point(parse_start:account.BaseReq)
  for (;;) {
    ::std::pair< ::google::protobuf::uint32, bool> p = input->ReadTagWithCutoff(127);
    tag = p.first;
    if (!p.second) goto handle_unusual;
    switch (::google::protobuf::internal::WireFormatLite::GetTagFieldNumber(tag)) {
      // required bytes sesKey = 1;
      case 1: {
        if (tag == 10) {
          DO_(::google::protobuf::internal::WireFormatLite::ReadBytes(
                input, this->mutable_seskey()));
        } else {
          goto handle_unusual;
        }
        if (input->ExpectTag(16)) goto parse_accId;
        break;
      }

      // required uint32 accId = 2;
      case 2: {
        if (tag == 16) {
         parse_accId:
          DO_((::google::protobuf::internal::WireFormatLite::ReadPrimitive<
                   ::google::protobuf::uint32, ::google::protobuf::internal::WireFormatLite::TYPE_UINT32>(
                 input, &accid_)));
          set_has_accid();
        } else {
          goto handle_unusual;
        }
        if (input->ExpectAtEnd()) goto success;
        break;
      }

      default: {
      handle_unusual:
        if (tag == 0 ||
            ::google::protobuf::internal::WireFormatLite::GetTagWireType(tag) ==
            ::google::protobuf::internal::WireFormatLite::WIRETYPE_END_GROUP) {
          goto success;
        }
        DO_(::google::protobuf::internal::WireFormat::SkipField(
              input, tag, mutable_unknown_fields()));
        break;
      }
    }
  }
success:
  // @@protoc_insertion_point(parse_success:account.BaseReq)
  return true;
failure:
  // @@protoc_insertion_point(parse_failure:account.BaseReq)
  return false;
#undef DO_
}

void BaseReq::SerializeWithCachedSizes(
    ::google::protobuf::io::CodedOutputStream* output) const {
  // @@protoc_insertion_point(serialize_start:account.BaseReq)
  // required bytes sesKey = 1;
  if (has_seskey()) {
    ::google::protobuf::internal::WireFormatLite::WriteBytesMaybeAliased(
      1, this->seskey(), output);
  }

  // required uint32 accId = 2;
  if (has_accid()) {
    ::google::protobuf::internal::WireFormatLite::WriteUInt32(2, this->accid(), output);
  }

  if (!unknown_fields().empty()) {
    ::google::protobuf::internal::WireFormat::SerializeUnknownFields(
        unknown_fields(), output);
  }
  // @@protoc_insertion_point(serialize_end:account.BaseReq)
}

::google::protobuf::uint8* BaseReq::SerializeWithCachedSizesToArray(
    ::google::protobuf::uint8* target) const {
  // @@protoc_insertion_point(serialize_to_array_start:account.BaseReq)
  // required bytes sesKey = 1;
  if (has_seskey()) {
    target =
      ::google::protobuf::internal::WireFormatLite::WriteBytesToArray(
        1, this->seskey(), target);
  }

  // required uint32 accId = 2;
  if (has_accid()) {
    target = ::google::protobuf::internal::WireFormatLite::WriteUInt32ToArray(2, this->accid(), target);
  }

  if (!unknown_fields().empty()) {
    target = ::google::protobuf::internal::WireFormat::SerializeUnknownFieldsToArray(
        unknown_fields(), target);
  }
  // @@protoc_insertion_point(serialize_to_array_end:account.BaseReq)
  return target;
}

int BaseReq::ByteSize() const {
  int total_size = 0;

  if (_has_bits_[0 / 32] & (0xffu << (0 % 32))) {
    // required bytes sesKey = 1;
    if (has_seskey()) {
      total_size += 1 +
        ::google::protobuf::internal::WireFormatLite::BytesSize(
          this->seskey());
    }

    // required uint32 accId = 2;
    if (has_accid()) {
      total_size += 1 +
        ::google::protobuf::internal::WireFormatLite::UInt32Size(
          this->accid());
    }

  }
  if (!unknown_fields().empty()) {
    total_size +=
      ::google::protobuf::internal::WireFormat::ComputeUnknownFieldsSize(
        unknown_fields());
  }
  GOOGLE_SAFE_CONCURRENT_WRITES_BEGIN();
  _cached_size_ = total_size;
  GOOGLE_SAFE_CONCURRENT_WRITES_END();
  return total_size;
}

void BaseReq::MergeFrom(const ::google::protobuf::Message& from) {
  GOOGLE_CHECK_NE(&from, this);
  const BaseReq* source =
    ::google::protobuf::internal::dynamic_cast_if_available<const BaseReq*>(
      &from);
  if (source == NULL) {
    ::google::protobuf::internal::ReflectionOps::Merge(from, this);
  } else {
    MergeFrom(*source);
  }
}

void BaseReq::MergeFrom(const BaseReq& from) {
  GOOGLE_CHECK_NE(&from, this);
  if (from._has_bits_[0 / 32] & (0xffu << (0 % 32))) {
    if (from.has_seskey()) {
      set_seskey(from.seskey());
    }
    if (from.has_accid()) {
      set_accid(from.accid());
    }
  }
  mutable_unknown_fields()->MergeFrom(from.unknown_fields());
}

void BaseReq::CopyFrom(const ::google::protobuf::Message& from) {
  if (&from == this) return;
  Clear();
  MergeFrom(from);
}

void BaseReq::CopyFrom(const BaseReq& from) {
  if (&from == this) return;
  Clear();
  MergeFrom(from);
}

bool BaseReq::IsInitialized() const {
  if ((_has_bits_[0] & 0x00000003) != 0x00000003) return false;

  return true;
}

void BaseReq::Swap(BaseReq* other) {
  if (other != this) {
    std::swap(seskey_, other->seskey_);
    std::swap(accid_, other->accid_);
    std::swap(_has_bits_[0], other->_has_bits_[0]);
    _unknown_fields_.Swap(&other->_unknown_fields_);
    std::swap(_cached_size_, other->_cached_size_);
  }
}

::google::protobuf::Metadata BaseReq::GetMetadata() const {
  protobuf_AssignDescriptorsOnce();
  ::google::protobuf::Metadata metadata;
  metadata.descriptor = BaseReq_descriptor_;
  metadata.reflection = BaseReq_reflection_;
  return metadata;
}


// ===================================================================

#ifndef _MSC_VER
const int PingRequest::kBaseReqFieldNumber;
#endif  // !_MSC_VER

PingRequest::PingRequest()
  : ::google::protobuf::Message() {
  SharedCtor();
  // @@protoc_insertion_point(constructor:account.PingRequest)
}

void PingRequest::InitAsDefaultInstance() {
  basereq_ = const_cast< ::account::BaseReq*>(&::account::BaseReq::default_instance());
}

PingRequest::PingRequest(const PingRequest& from)
  : ::google::protobuf::Message() {
  SharedCtor();
  MergeFrom(from);
  // @@protoc_insertion_point(copy_constructor:account.PingRequest)
}

void PingRequest::SharedCtor() {
  _cached_size_ = 0;
  basereq_ = NULL;
  ::memset(_has_bits_, 0, sizeof(_has_bits_));
}

PingRequest::~PingRequest() {
  // @@protoc_insertion_point(destructor:account.PingRequest)
  SharedDtor();
}

void PingRequest::SharedDtor() {
  if (this != default_instance_) {
    delete basereq_;
  }
}

void PingRequest::SetCachedSize(int size) const {
  GOOGLE_SAFE_CONCURRENT_WRITES_BEGIN();
  _cached_size_ = size;
  GOOGLE_SAFE_CONCURRENT_WRITES_END();
}
const ::google::protobuf::Descriptor* PingRequest::descriptor() {
  protobuf_AssignDescriptorsOnce();
  return PingRequest_descriptor_;
}

const PingRequest& PingRequest::default_instance() {
  if (default_instance_ == NULL) protobuf_AddDesc_common_2eproto();
  return *default_instance_;
}

PingRequest* PingRequest::default_instance_ = NULL;

PingRequest* PingRequest::New() const {
  return new PingRequest;
}

void PingRequest::Clear() {
  if (has_basereq()) {
    if (basereq_ != NULL) basereq_->::account::BaseReq::Clear();
  }
  ::memset(_has_bits_, 0, sizeof(_has_bits_));
  mutable_unknown_fields()->Clear();
}

bool PingRequest::MergePartialFromCodedStream(
    ::google::protobuf::io::CodedInputStream* input) {
#define DO_(EXPRESSION) if (!(EXPRESSION)) goto failure
  ::google::protobuf::uint32 tag;
  // @@protoc_insertion_point(parse_start:account.PingRequest)
  for (;;) {
    ::std::pair< ::google::protobuf::uint32, bool> p = input->ReadTagWithCutoff(127);
    tag = p.first;
    if (!p.second) goto handle_unusual;
    switch (::google::protobuf::internal::WireFormatLite::GetTagFieldNumber(tag)) {
      // required .account.BaseReq baseReq = 1;
      case 1: {
        if (tag == 10) {
          DO_(::google::protobuf::internal::WireFormatLite::ReadMessageNoVirtual(
               input, mutable_basereq()));
        } else {
          goto handle_unusual;
        }
        if (input->ExpectAtEnd()) goto success;
        break;
      }

      default: {
      handle_unusual:
        if (tag == 0 ||
            ::google::protobuf::internal::WireFormatLite::GetTagWireType(tag) ==
            ::google::protobuf::internal::WireFormatLite::WIRETYPE_END_GROUP) {
          goto success;
        }
        DO_(::google::protobuf::internal::WireFormat::SkipField(
              input, tag, mutable_unknown_fields()));
        break;
      }
    }
  }
success:
  // @@protoc_insertion_point(parse_success:account.PingRequest)
  return true;
failure:
  // @@protoc_insertion_point(parse_failure:account.PingRequest)
  return false;
#undef DO_
}

void PingRequest::SerializeWithCachedSizes(
    ::google::protobuf::io::CodedOutputStream* output) const {
  // @@protoc_insertion_point(serialize_start:account.PingRequest)
  // required .account.BaseReq baseReq = 1;
  if (has_basereq()) {
    ::google::protobuf::internal::WireFormatLite::WriteMessageMaybeToArray(
      1, this->basereq(), output);
  }

  if (!unknown_fields().empty()) {
    ::google::protobuf::internal::WireFormat::SerializeUnknownFields(
        unknown_fields(), output);
  }
  // @@protoc_insertion_point(serialize_end:account.PingRequest)
}

::google::protobuf::uint8* PingRequest::SerializeWithCachedSizesToArray(
    ::google::protobuf::uint8* target) const {
  // @@protoc_insertion_point(serialize_to_array_start:account.PingRequest)
  // required .account.BaseReq baseReq = 1;
  if (has_basereq()) {
    target = ::google::protobuf::internal::WireFormatLite::
      WriteMessageNoVirtualToArray(
        1, this->basereq(), target);
  }

  if (!unknown_fields().empty()) {
    target = ::google::protobuf::internal::WireFormat::SerializeUnknownFieldsToArray(
        unknown_fields(), target);
  }
  // @@protoc_insertion_point(serialize_to_array_end:account.PingRequest)
  return target;
}

int PingRequest::ByteSize() const {
  int total_size = 0;

  if (_has_bits_[0 / 32] & (0xffu << (0 % 32))) {
    // required .account.BaseReq baseReq = 1;
    if (has_basereq()) {
      total_size += 1 +
        ::google::protobuf::internal::WireFormatLite::MessageSizeNoVirtual(
          this->basereq());
    }

  }
  if (!unknown_fields().empty()) {
    total_size +=
      ::google::protobuf::internal::WireFormat::ComputeUnknownFieldsSize(
        unknown_fields());
  }
  GOOGLE_SAFE_CONCURRENT_WRITES_BEGIN();
  _cached_size_ = total_size;
  GOOGLE_SAFE_CONCURRENT_WRITES_END();
  return total_size;
}

void PingRequest::MergeFrom(const ::google::protobuf::Message& from) {
  GOOGLE_CHECK_NE(&from, this);
  const PingRequest* source =
    ::google::protobuf::internal::dynamic_cast_if_available<const PingRequest*>(
      &from);
  if (source == NULL) {
    ::google::protobuf::internal::ReflectionOps::Merge(from, this);
  } else {
    MergeFrom(*source);
  }
}

void PingRequest::MergeFrom(const PingRequest& from) {
  GOOGLE_CHECK_NE(&from, this);
  if (from._has_bits_[0 / 32] & (0xffu << (0 % 32))) {
    if (from.has_basereq()) {
      mutable_basereq()->::account::BaseReq::MergeFrom(from.basereq());
    }
  }
  mutable_unknown_fields()->MergeFrom(from.unknown_fields());
}

void PingRequest::CopyFrom(const ::google::protobuf::Message& from) {
  if (&from == this) return;
  Clear();
  MergeFrom(from);
}

void PingRequest::CopyFrom(const PingRequest& from) {
  if (&from == this) return;
  Clear();
  MergeFrom(from);
}

bool PingRequest::IsInitialized() const {
  if ((_has_bits_[0] & 0x00000001) != 0x00000001) return false;

  if (has_basereq()) {
    if (!this->basereq().IsInitialized()) return false;
  }
  return true;
}

void PingRequest::Swap(PingRequest* other) {
  if (other != this) {
    std::swap(basereq_, other->basereq_);
    std::swap(_has_bits_[0], other->_has_bits_[0]);
    _unknown_fields_.Swap(&other->_unknown_fields_);
    std::swap(_cached_size_, other->_cached_size_);
  }
}

::google::protobuf::Metadata PingRequest::GetMetadata() const {
  protobuf_AssignDescriptorsOnce();
  ::google::protobuf::Metadata metadata;
  metadata.descriptor = PingRequest_descriptor_;
  metadata.reflection = PingRequest_reflection_;
  return metadata;
}


// ===================================================================

#ifndef _MSC_VER
#endif  // !_MSC_VER

PongResponse::PongResponse()
  : ::google::protobuf::Message() {
  SharedCtor();
  // @@protoc_insertion_point(constructor:account.PongResponse)
}

void PongResponse::InitAsDefaultInstance() {
}

PongResponse::PongResponse(const PongResponse& from)
  : ::google::protobuf::Message() {
  SharedCtor();
  MergeFrom(from);
  // @@protoc_insertion_point(copy_constructor:account.PongResponse)
}

void PongResponse::SharedCtor() {
  _cached_size_ = 0;
  ::memset(_has_bits_, 0, sizeof(_has_bits_));
}

PongResponse::~PongResponse() {
  // @@protoc_insertion_point(destructor:account.PongResponse)
  SharedDtor();
}

void PongResponse::SharedDtor() {
  if (this != default_instance_) {
  }
}

void PongResponse::SetCachedSize(int size) const {
  GOOGLE_SAFE_CONCURRENT_WRITES_BEGIN();
  _cached_size_ = size;
  GOOGLE_SAFE_CONCURRENT_WRITES_END();
}
const ::google::protobuf::Descriptor* PongResponse::descriptor() {
  protobuf_AssignDescriptorsOnce();
  return PongResponse_descriptor_;
}

const PongResponse& PongResponse::default_instance() {
  if (default_instance_ == NULL) protobuf_AddDesc_common_2eproto();
  return *default_instance_;
}

PongResponse* PongResponse::default_instance_ = NULL;

PongResponse* PongResponse::New() const {
  return new PongResponse;
}

void PongResponse::Clear() {
  ::memset(_has_bits_, 0, sizeof(_has_bits_));
  mutable_unknown_fields()->Clear();
}

bool PongResponse::MergePartialFromCodedStream(
    ::google::protobuf::io::CodedInputStream* input) {
#define DO_(EXPRESSION) if (!(EXPRESSION)) goto failure
  ::google::protobuf::uint32 tag;
  // @@protoc_insertion_point(parse_start:account.PongResponse)
  for (;;) {
    ::std::pair< ::google::protobuf::uint32, bool> p = input->ReadTagWithCutoff(127);
    tag = p.first;
    if (!p.second) goto handle_unusual;
  handle_unusual:
    if (tag == 0 ||
        ::google::protobuf::internal::WireFormatLite::GetTagWireType(tag) ==
        ::google::protobuf::internal::WireFormatLite::WIRETYPE_END_GROUP) {
      goto success;
    }
    DO_(::google::protobuf::internal::WireFormat::SkipField(
          input, tag, mutable_unknown_fields()));
  }
success:
  // @@protoc_insertion_point(parse_success:account.PongResponse)
  return true;
failure:
  // @@protoc_insertion_point(parse_failure:account.PongResponse)
  return false;
#undef DO_
}

void PongResponse::SerializeWithCachedSizes(
    ::google::protobuf::io::CodedOutputStream* output) const {
  // @@protoc_insertion_point(serialize_start:account.PongResponse)
  if (!unknown_fields().empty()) {
    ::google::protobuf::internal::WireFormat::SerializeUnknownFields(
        unknown_fields(), output);
  }
  // @@protoc_insertion_point(serialize_end:account.PongResponse)
}

::google::protobuf::uint8* PongResponse::SerializeWithCachedSizesToArray(
    ::google::protobuf::uint8* target) const {
  // @@protoc_insertion_point(serialize_to_array_start:account.PongResponse)
  if (!unknown_fields().empty()) {
    target = ::google::protobuf::internal::WireFormat::SerializeUnknownFieldsToArray(
        unknown_fields(), target);
  }
  // @@protoc_insertion_point(serialize_to_array_end:account.PongResponse)
  return target;
}

int PongResponse::ByteSize() const {
  int total_size = 0;

  if (!unknown_fields().empty()) {
    total_size +=
      ::google::protobuf::internal::WireFormat::ComputeUnknownFieldsSize(
        unknown_fields());
  }
  GOOGLE_SAFE_CONCURRENT_WRITES_BEGIN();
  _cached_size_ = total_size;
  GOOGLE_SAFE_CONCURRENT_WRITES_END();
  return total_size;
}

void PongResponse::MergeFrom(const ::google::protobuf::Message& from) {
  GOOGLE_CHECK_NE(&from, this);
  const PongResponse* source =
    ::google::protobuf::internal::dynamic_cast_if_available<const PongResponse*>(
      &from);
  if (source == NULL) {
    ::google::protobuf::internal::ReflectionOps::Merge(from, this);
  } else {
    MergeFrom(*source);
  }
}

void PongResponse::MergeFrom(const PongResponse& from) {
  GOOGLE_CHECK_NE(&from, this);
  mutable_unknown_fields()->MergeFrom(from.unknown_fields());
}

void PongResponse::CopyFrom(const ::google::protobuf::Message& from) {
  if (&from == this) return;
  Clear();
  MergeFrom(from);
}

void PongResponse::CopyFrom(const PongResponse& from) {
  if (&from == this) return;
  Clear();
  MergeFrom(from);
}

bool PongResponse::IsInitialized() const {

  return true;
}

void PongResponse::Swap(PongResponse* other) {
  if (other != this) {
    _unknown_fields_.Swap(&other->_unknown_fields_);
    std::swap(_cached_size_, other->_cached_size_);
  }
}

::google::protobuf::Metadata PongResponse::GetMetadata() const {
  protobuf_AssignDescriptorsOnce();
  ::google::protobuf::Metadata metadata;
  metadata.descriptor = PongResponse_descriptor_;
  metadata.reflection = PongResponse_reflection_;
  return metadata;
}


// @@protoc_insertion_point(namespace_scope)

}  // namespace account

// @@protoc_insertion_point(global_scope)
