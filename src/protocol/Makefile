NANOPB_DIR = ../../dependencies/nanopb
NANOPB_PROTO_DIR = $(NANOPB_DIR)/generator/proto

PROTOC = protoc
PROTOC_OPTS = --plugin=protoc-gen-nanopb=protoc-gen-nanopb \
              -I$(NANOPB_PROTO_DIR) -I. --nanopb_out=.

gen: make-nanopb criterion.pb.c

make-nanopb:
	$(MAKE) -C $(NANOPB_PROTO_DIR)

criterion.pb.c: criterion.proto
	$(PROTOC) $(PROTOC_OPTS) $<

.PHONY: gen make-nanopb
