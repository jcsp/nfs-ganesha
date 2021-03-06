/*
 * vim:noexpandtab:shiftwidth=8:tabstop=8:
 *
 * Copyright CEA/DAM/DIF  (2011)
 * contributeur : Philippe DENIEL   philippe.deniel@cea.fr
 *                Thomas LEIBOVICI  thomas.leibovici@cea.fr
 *
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 3 of the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301 USA
 *
 * ---------------------------------------
 */

/**
 * \file    9p_proto_tools.c
 * \brief   9P version
 *
 * 9p_proto_tools.c : _9P_interpretor, protocol's service functions
 *
 */

#include "config.h"
#include <stdio.h>
#include <string.h>
#include <pthread.h>
#include "nfs_core.h"
#include "log.h"
#include "9p.h"
#include "config_parsing.h"

int _9p_read_conf(config_file_t in_config, _9p_parameter_t *pparam)
{
	int var_max;
	int var_index;
	int err;
	char *key_name;
	char *key_value;
	config_item_t block;

	/* Is the config tree initialized ? */
	if (in_config == NULL || pparam == NULL)
		return -1;

	/* Get the config BLOCK */
	block = config_FindItemByName(in_config, CONF_LABEL_9P);
	if (block == NULL)
		return -2;	/* no 9P config, use default */
	else if (config_ItemType(block) != CONFIG_ITEM_BLOCK)
		return -1;

	var_max = config_GetNbItems(block);

	for (var_index = 0; var_index < var_max; var_index++) {
		config_item_t item;

		item = config_GetItemByIndex(block, var_index);

		/* Get key's name */
		err = config_GetKeyValue(item, &key_name, &key_value);
		if (err != 0) {
			fprintf(stderr,
				"Error reading key[%d] from section \"%s\" of configuration file.\n",
				var_index, CONF_LABEL_9P);
			return -1;
		}
		if (!strcasecmp(key_name, "_9P_TCP_Port")) {
			pparam->_9p_tcp_port = atoi(key_value);
		} else if (!strcasecmp(key_name, "_9P_RDMA_Port")) {
			pparam->_9p_rdma_port = atoi(key_value);
		} else if (!strcasecmp(key_name, "_9P_TCP_Msize")) {
			pparam->_9p_tcp_msize = atoi(key_value);
		} else if (!strcasecmp(key_name, "_9P_RDMA_Msize")) {
			pparam->_9p_rdma_msize = atoi(key_value);
		} else if (!strcasecmp(key_name, "_9P_RDMA_Backlog")) {
			pparam->_9p_rdma_backlog = atoi(key_value);
		} else if (!strcasecmp(key_name, "DebugLevel")
			   || !strcasecmp(key_name, "LogFile")) {
			fprintf(stderr, "Deprecated 9P option %s=\'%s\'",
				key_name, key_value);
		} else {
			fprintf(stderr,
				"Unknown or unsettable key: %s (item %s)\n",
				key_name, CONF_LABEL_9P);
			return -1;
		}
	}			/* for */

	return 0;
}				/* _9p_read_conf */
