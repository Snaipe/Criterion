NANOPB_DIR = ../../subprojects/nanopb
NANOPB_PROTO_DIR = $(NANOPB_DIR)/generator/proto

PROTOC = protoc
PROTOC_OPTS = --plugin=protoc-gen-nanopb=$(NANOPB_DIR)/generator/protoc-gen-nanopb \
              -I$(NANOPB_PROTO_DIR) -I. --nanopb_out=.

gen: make-nanopb criterion.pb.c

make-nanopb:
	$(MAKE) -C $(NANOPB_PROTO_DIR)

criterion.pb.c: criterion.proto
	$(PROTOC) $(PROTOC_OPTS) $<

.PHONY: gen make-nanopb
