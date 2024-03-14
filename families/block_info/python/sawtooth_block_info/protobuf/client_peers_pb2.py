# Generated by the protocol buffer compiler.  DO NOT EDIT!
# source: python/sawtooth_block_info/protobuf/client_peers.proto

import sys
_b=sys.version_info[0]<3 and (lambda x:x) or (lambda x:x.encode('latin1'))
from google.protobuf import descriptor as _descriptor
from google.protobuf import message as _message
from google.protobuf import reflection as _reflection
from google.protobuf import symbol_database as _symbol_database
# @@protoc_insertion_point(imports)

_sym_db = _symbol_database.Default()




DESCRIPTOR = _descriptor.FileDescriptor(
  name='python/sawtooth_block_info/protobuf/client_peers.proto',
  package='',
  syntax='proto3',
  serialized_options=_b('\n\025sawtooth.sdk.protobufP\001Z\013client_peer'),
  serialized_pb=_b('\n6python/sawtooth_block_info/protobuf/client_peers.proto\"\x17\n\x15\x43lientPeersGetRequest\"\x86\x01\n\x16\x43lientPeersGetResponse\x12.\n\x06status\x18\x01 \x01(\x0e\x32\x1e.ClientPeersGetResponse.Status\x12\r\n\x05peers\x18\x02 \x03(\t\"-\n\x06Status\x12\x10\n\x0cSTATUS_UNSET\x10\x00\x12\x06\n\x02OK\x10\x01\x12\t\n\x05\x45RROR\x10\x02\x42&\n\x15sawtooth.sdk.protobufP\x01Z\x0b\x63lient_peerb\x06proto3')
)



_CLIENTPEERSGETRESPONSE_STATUS = _descriptor.EnumDescriptor(
  name='Status',
  full_name='ClientPeersGetResponse.Status',
  filename=None,
  file=DESCRIPTOR,
  values=[
    _descriptor.EnumValueDescriptor(
      name='STATUS_UNSET', index=0, number=0,
      serialized_options=None,
      type=None),
    _descriptor.EnumValueDescriptor(
      name='OK', index=1, number=1,
      serialized_options=None,
      type=None),
    _descriptor.EnumValueDescriptor(
      name='ERROR', index=2, number=2,
      serialized_options=None,
      type=None),
  ],
  containing_type=None,
  serialized_options=None,
  serialized_start=173,
  serialized_end=218,
)
_sym_db.RegisterEnumDescriptor(_CLIENTPEERSGETRESPONSE_STATUS)


_CLIENTPEERSGETREQUEST = _descriptor.Descriptor(
  name='ClientPeersGetRequest',
  full_name='ClientPeersGetRequest',
  filename=None,
  file=DESCRIPTOR,
  containing_type=None,
  fields=[
  ],
  extensions=[
  ],
  nested_types=[],
  enum_types=[
  ],
  serialized_options=None,
  is_extendable=False,
  syntax='proto3',
  extension_ranges=[],
  oneofs=[
  ],
  serialized_start=58,
  serialized_end=81,
)


_CLIENTPEERSGETRESPONSE = _descriptor.Descriptor(
  name='ClientPeersGetResponse',
  full_name='ClientPeersGetResponse',
  filename=None,
  file=DESCRIPTOR,
  containing_type=None,
  fields=[
    _descriptor.FieldDescriptor(
      name='status', full_name='ClientPeersGetResponse.status', index=0,
      number=1, type=14, cpp_type=8, label=1,
      has_default_value=False, default_value=0,
      message_type=None, enum_type=None, containing_type=None,
      is_extension=False, extension_scope=None,
      serialized_options=None, file=DESCRIPTOR),
    _descriptor.FieldDescriptor(
      name='peers', full_name='ClientPeersGetResponse.peers', index=1,
      number=2, type=9, cpp_type=9, label=3,
      has_default_value=False, default_value=[],
      message_type=None, enum_type=None, containing_type=None,
      is_extension=False, extension_scope=None,
      serialized_options=None, file=DESCRIPTOR),
  ],
  extensions=[
  ],
  nested_types=[],
  enum_types=[
    _CLIENTPEERSGETRESPONSE_STATUS,
  ],
  serialized_options=None,
  is_extendable=False,
  syntax='proto3',
  extension_ranges=[],
  oneofs=[
  ],
  serialized_start=84,
  serialized_end=218,
)

_CLIENTPEERSGETRESPONSE.fields_by_name['status'].enum_type = _CLIENTPEERSGETRESPONSE_STATUS
_CLIENTPEERSGETRESPONSE_STATUS.containing_type = _CLIENTPEERSGETRESPONSE
DESCRIPTOR.message_types_by_name['ClientPeersGetRequest'] = _CLIENTPEERSGETREQUEST
DESCRIPTOR.message_types_by_name['ClientPeersGetResponse'] = _CLIENTPEERSGETRESPONSE
_sym_db.RegisterFileDescriptor(DESCRIPTOR)

ClientPeersGetRequest = _reflection.GeneratedProtocolMessageType('ClientPeersGetRequest', (_message.Message,), dict(
  DESCRIPTOR = _CLIENTPEERSGETREQUEST,
  __module__ = 'python.sawtooth_block_info.protobuf.client_peers_pb2'
  # @@protoc_insertion_point(class_scope:ClientPeersGetRequest)
  ))
_sym_db.RegisterMessage(ClientPeersGetRequest)

ClientPeersGetResponse = _reflection.GeneratedProtocolMessageType('ClientPeersGetResponse', (_message.Message,), dict(
  DESCRIPTOR = _CLIENTPEERSGETRESPONSE,
  __module__ = 'python.sawtooth_block_info.protobuf.client_peers_pb2'
  # @@protoc_insertion_point(class_scope:ClientPeersGetResponse)
  ))
_sym_db.RegisterMessage(ClientPeersGetResponse)


DESCRIPTOR._options = None
# @@protoc_insertion_point(module_scope)