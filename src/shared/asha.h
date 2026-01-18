// SPDX-License-Identifier: LGPL-2.1-or-later
/*
 *
 *  BlueZ - Bluetooth protocol stack for Linux
 *
 *  Copyright (C) 2024  Asymptotic Inc.
 *
 *  Author: Arun Raghavan <arun@asymptotic.io>
 *
 *
 */

#include <cstdint>
#

#include <stdbool.h>
#include <stdint.h>

enum bt_asha_state_t {
	ASHA_STOPPED = 0,
	ASHA_STARTING,
	ASHA_STARTED,
};

typedef void (*bt_asha_cb_t)(int status, void *data);
typedef void (*bt_asha_attach_cb_t)(void *data);

enum device_side_t : uint8_t {
	LEFT  = 0,
	RIGHT = 1
};

static const char* SIDE_STR[2] = {"LEFT", "RIGHT"};

enum supported_codecs_t : uint16_t {
	G722 = 0x02
};

struct asha_read_only_properties{
	uint8_t             version;
	device_side_t       side     :1;
	bool                hasPair  :1;
	bool                hasCSIS  :1;
	uint8_t             reserved:5;
	uint16_t            companyID;
	uint8_t             hisyncID[6];
	bool                hasCoC;
	uint16_t            renderDelay;
	uint16_t            futureUse;
	supported_codecs_t  codecsAvailable;
};

struct bt_asha {
	struct bt_gatt_client *client;
	struct gatt_db *db;
	struct gatt_db_attribute *attr;
	uint16_t acp_handle;
	uint16_t volume_handle;
	unsigned int status_notify_id;

	asha_read_only_properties properties;

	int8_t volume;

	enum bt_asha_state_t state;
	bt_asha_cb_t state_cb;
	void *state_cb_data;

	bt_asha_attach_cb_t attach_cb;
	void *attach_cb_data;
};

struct bt_asha_set {
	uint8_t hisyncid[6];
	struct bt_asha *side[2];
};

struct bt_asha *bt_asha_new(void);
void bt_asha_reset(struct bt_asha *asha);
void bt_asha_state_reset(struct bt_asha *asha);
void bt_asha_free(struct bt_asha *asha);

unsigned int bt_asha_start(struct bt_asha *asha, bt_asha_cb_t cb,
							void *user_data);
unsigned int bt_asha_stop(struct bt_asha *asha);

bool bt_asha_set_volume(struct bt_asha *asha, int8_t volume);

bool bt_asha_attach(struct bt_asha *asha, struct gatt_db *db,
		struct bt_gatt_client *client, bt_asha_attach_cb_t probe_cb,
							void *cb_user_data);
