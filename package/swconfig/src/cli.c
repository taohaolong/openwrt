/*
 * swconfig.c: Switch configuration utility
 *
 * Copyright (C) 2008 Felix Fietkau <nbd@openwrt.org>
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * version 2 as published by the Free Software Foundatio.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 */

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <inttypes.h>
#include <errno.h>
#include <stdint.h>
#include <getopt.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <uci.h>

#include <linux/types.h>
#include <linux/netlink.h>
#include <linux/genetlink.h>
#include <netlink/netlink.h>
#include <netlink/genl/genl.h>
#include <netlink/genl/ctrl.h>
#include <linux/switch.h>
#include "swlib.h"

enum {
	CMD_NONE,
	CMD_GET,
	CMD_SET,
	CMD_LOAD,
	CMD_HELP,
};

static void
print_attrs(const struct switch_attr *attr)
{
	int i = 0;
	while (attr) {
		const char *type;
		switch(attr->type) {
			case SWITCH_TYPE_INT:
				type = "int";
				break;
			case SWITCH_TYPE_STRING:
				type = "string";
				break;
			case SWITCH_TYPE_PORTS:
				type = "ports";
				break;
			case SWITCH_TYPE_NOVAL:
				type = "none";
				break;
			default:
				type = "unknown";
				break;
		}
		printf("\tAttribute %d (%s): %s (%s)\n", ++i, type, attr->name, attr->description);
		attr = attr->next;
	}
}

static void
list_attributes(struct switch_dev *dev)
{
	printf("Switch %d: %s(%s), ports: %d, vlans: %d\n", dev->id, dev->dev_name, dev->name, dev->ports, dev->vlans);
	printf("     --switch\n");
	print_attrs(dev->ops);
	printf("     --vlan\n");
	print_attrs(dev->vlan_ops);
	printf("     --port\n");
	print_attrs(dev->port_ops);
}

static void
print_usage(void)
{
	printf("swconfig dev <dev> [port <port>|vlan <vlan>] (help|set <key> <value>|get <key>|load <config>)\n");
	exit(1);
}

static void
swconfig_load_uci(struct switch_dev *dev, const char *name)
{
	struct uci_context *ctx;
	struct uci_package *p = NULL;
	struct uci_element *e;
	int ret = -1;

	ctx = uci_alloc_context();
	if (!ctx)
		return;

	uci_load(ctx, name, &p);
	if (!p) {
		uci_perror(ctx, "Failed to load config file: ");
		goto out;
	}

	ret = swlib_apply_from_uci(dev, p);
	if (ret < 0)
		fprintf(stderr, "Failed to apply configuration for switch '%s'\n", dev->dev_name);

out:
	uci_free_context(ctx);
	exit(ret);
}

int main(int argc, char **argv)
{
	int retval = 0;
	struct switch_dev *dev;
	struct switch_attr *a;
	struct switch_val val;
	int err;
	int i;

	struct switch_port *ports;

	int cmd = CMD_NONE;
	char *cdev = NULL;
	int cport = -1;
	int cvlan = -1;
	char *ckey = NULL;
	char *cvalue = NULL;

	if(argc < 4)
		print_usage();

	if(strcmp(argv[1], "dev"))
		print_usage();

	cdev = argv[2];

	for(i = 3; i < argc; i++)
	{
		char *arg = argv[i];
		if (cmd != CMD_NONE) {
			print_usage();
		} else if (!strcmp(arg, "port") && i+1 < argc) {
			cport = atoi(argv[++i]);
		} else if (!strcmp(arg, "vlan") && i+1 < argc) {
			cvlan = atoi(argv[++i]);
		} else if (!strcmp(arg, "help")) {
			cmd = CMD_HELP;
		} else if (!strcmp(arg, "set") && i+1 < argc) {
			cmd = CMD_SET;
			ckey = argv[++i];
			if (i+1 < argc)
				cvalue = argv[++i];
		} else if (!strcmp(arg, "get") && i+1 < argc) {
			cmd = CMD_GET;
			ckey = argv[++i];
		} else if (!strcmp(arg, "load") && i+1 < argc) {
			if ((cport >= 0) || (cvlan >= 0))
				print_usage();
			cmd = CMD_LOAD;
			ckey = argv[++i];
		} else {
			print_usage();
		}
	}

	if (cmd == CMD_NONE)
		print_usage();
	if (cport > -1 && cvlan > -1)
		print_usage();

	dev = swlib_connect(cdev);
	if (!dev) {
		fprintf(stderr, "Failed to connect to the switch\n");
		return 1;
	}

	ports = malloc(sizeof(struct switch_port) * dev->ports);
	memset(ports, 0, sizeof(struct switch_port) * dev->ports);
	swlib_scan(dev);

	if (cmd == CMD_GET || cmd == CMD_SET) {
		if(cport > -1)
			a = swlib_lookup_attr(dev, SWLIB_ATTR_GROUP_PORT, ckey);
		else if(cvlan > -1)
			a = swlib_lookup_attr(dev, SWLIB_ATTR_GROUP_VLAN, ckey);
		else
			a = swlib_lookup_attr(dev, SWLIB_ATTR_GROUP_GLOBAL, ckey);

		if(!a)
		{
			fprintf(stderr, "Unknown attribute \"%s\"\n", ckey);
			goto out;
		}
	}

	switch(cmd)
	{
	case CMD_SET:
		if ((a->type != SWITCH_TYPE_NOVAL) &&
				(cvalue == NULL))
			print_usage();

		if(cvlan > -1)
			cport = cvlan;

		if(swlib_set_attr_string(dev, a, cport, cvalue) < 0)
		{
			fprintf(stderr, "failed\n");
			retval = -1;
			goto out;
		}
		break;
	case CMD_GET:
		if(cvlan > -1)
			val.port_vlan = cvlan;
		if(cport > -1)
			val.port_vlan = cport;
		if(swlib_get_attr(dev, a, &val) < 0)
		{
			fprintf(stderr, "failed\n");
			retval = -1;
			goto out;
		}
		switch(a->type) {
		case SWITCH_TYPE_INT:
			printf("%d\n", val.value.i);
			break;
		case SWITCH_TYPE_STRING:
			printf("%s\n", val.value.s);
			break;
		case SWITCH_TYPE_PORTS:
			for(i = 0; i < val.len; i++) {
				printf("%d%s ",
					val.value.ports[i].id,
					(val.value.ports[i].flags &
					 SWLIB_PORT_FLAG_TAGGED) ? "t" : "");
			}
			printf("\n");
			break;
		}
		break;
	case CMD_LOAD:
		swconfig_load_uci(dev, ckey);
		break;
	case CMD_HELP:
		list_attributes(dev);
		break;
	}

out:
	swlib_free_all(dev);
	free(ports);

	return 0;
}
