// -*- mode: c++; c-basic-offset: 4 -*-
/*
 * ipsumdumpinfo.{cc,hh} -- information used by IP summary dump elements
 * Eddie Kohler
 *
 * Copyright (c) 2002 International Computer Science Institute
 *
 * Permission is hereby granted, free of charge, to any person obtaining a
 * copy of this software and associated documentation files (the "Software"),
 * to deal in the Software without restriction, subject to the conditions
 * listed in the Click LICENSE file. These conditions include: you must
 * preserve this copyright notice, and you cannot mention the copyright
 * holders in advertising related to the Software without their permission.
 * The Software is provided WITHOUT ANY WARRANTY, EXPRESS OR IMPLIED. This
 * notice is a summary of the Click LICENSE file; the license in that file is
 * legally binding.
 */

#include <click/config.h>

#include "ipsumdumpinfo.hh"
CLICK_DECLS

static const char *content_names[] = {
    "??", "timestamp", "ts_sec", "ts_usec", "ip_src",
    "ip_dst", "ip_len", "ip_proto", "ip_id", "sport",
    "dport", "tcp_seq", "tcp_ack", "tcp_flags", "payload_len",
    "count", "ip_frag", "ip_fragoff", "payload", "direction",
    "aggregate", "tcp_sack", "tcp_opt", "tcp_ntopt", "first_timestamp",
    "tcp_window", "ip_opt", "ip_tos", "ip_ttl"
};

const char *
IPSummaryDumpInfo::unparse_content(int content)
{
    if (content < 0 || content >= (int)(sizeof(content_names) / sizeof(content_names[0])))
	return "??";
    else
	return content_names[content];
}

int
IPSummaryDumpInfo::parse_content(const String &word)
{
    if (word == "timestamp" || word == "ts")
	return W_TIMESTAMP;
    else if (word == "sec" || word == "ts_sec")
	return W_TIMESTAMP_SEC;
    else if (word == "usec" || word == "ts_usec")
	return W_TIMESTAMP_USEC;
    else if (word == "src" || word == "ip_src")
	return W_SRC;
    else if (word == "dst" || word == "ip_dst")
	return W_DST;
    else if (word == "sport")
	return W_SPORT;
    else if (word == "dport")
	return W_DPORT;
    else if (word == "frag" || word == "ip_frag")
	return W_FRAG;
    else if (word == "fragoff" || word == "ip_fragoff")
	return W_FRAGOFF;
    else if (word == "len" || word == "length" || word == "ip_len")
	return W_LENGTH;
    else if (word == "id" || word == "ip_id")
	return W_IPID;
    else if (word == "proto" || word == "ip_proto" || word == "ip_p")
	return W_PROTO;
    else if (word == "tcp_seq" || word == "tcp_seqno")
	return W_TCP_SEQ;
    else if (word == "tcp_ack" || word == "tcp_ackno")
	return W_TCP_ACK;
    else if (word == "tcp_flags")
	return W_TCP_FLAGS;
    else if (word == "tcp_sack")
	return W_TCP_SACK;
    else if (word == "tcp_opt")
	return W_TCP_OPT;
    else if (word == "tcp_ntopt")
	return W_TCP_NTOPT;
    else if (word == "payload_len" || word == "payload_length")
	return W_PAYLOAD_LENGTH;
    else if (word == "count" || word == "pkt_count" || word == "packet_count")
	return W_COUNT;
    else if (word == "payload")
	return W_PAYLOAD;
    else if (word == "link" || word == "direction")
	return W_LINK;
    else if (word == "aggregate" || word == "agg")
	return W_AGGREGATE;
    else if (word == "first_timestamp" || word == "first_ts")
	return W_FIRST_TIMESTAMP;
    else if (word == "tcp_window" || word == "tcp_win")
	return W_TCP_WINDOW;
    else if (word == "ip_opt")
	return W_IP_OPT;
    else if (word == "ip_tos")
	return W_IP_TOS;
    else if (word == "ip_ttl")
	return W_IP_TTL;
    else if (find(word, ' ') != word.end()) {
	const char *space = find(word, ' ');
	return parse_content(word.substring(word.begin(), space) + "_" + word.substring(space + 1, word.end()));
    } else
	return W_NONE;
}

static int content_binary_sizes[] = {
    -10000, 8, 4, 4, 4,	// W_NONE, W_TIMESTAMP, W_TS_SEC, W_TS_USEC, W_SRC
    4, 4, 1, 2, 2,	// W_DST, W_LENGTH, W_PROTO, W_IPID, W_SPORT
    2, 4, 4, 1, 4,	// W_DPORT, W_TCP_SEQ, W_TCP_ACK, W_TCP_FLAGS, W_PL_LEN
    4, 1, 2, -10000, 1,	// W_COUNT, W_FRAG, W_FRAGOFF, W_PAYLOAD, W_LINK
    4, 4, 4, 4, 8,      // W_AGGREGATE, W_TCP_SACK, W_TCP_OPT, W_TCP_NTOPT,
			// W_FIRST_TIMESTAMP
    2, 4, 1, 1		// W_TCP_WINDOW, W_IP_OPT, W_IP_TOS, W_IP_TTL
};

int
IPSummaryDumpInfo::content_binary_size(int content)
{
    if (content < 0 || content >= (int)(sizeof(content_binary_sizes) / sizeof(content_binary_sizes[0])))
	return -10000;
    else
	return content_binary_sizes[content];
}
    
const char * const IPSummaryDumpInfo::tcp_flags_word = "FSRPAUEWN";

const uint8_t IPSummaryDumpInfo::tcp_flag_mapping[256] = {
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, // 0x0-
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, // 0x1-
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, // 0x2-
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, // 0x3-
    0, 5, 0, 0, 0, 7, 1, 0, 0, 0, 0, 0, 0, 0, 9, 0, // 0x4-
    4, 0, 3, 2, 0, 6, 0, 8, 7, 8, 0, 0, 0, 0, 0, 0, // 0x5-
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, // 0x6-
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, // 0x7-
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, // 0x8-
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, // 0x9-
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, // 0xA-
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, // 0xB-
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, // 0xC-
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, // 0xD-
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, // 0xE-
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0  // 0xF-    
};

ELEMENT_REQUIRES(userlevel)
ELEMENT_PROVIDES(IPSummaryDumpInfo)
CLICK_ENDDECLS
