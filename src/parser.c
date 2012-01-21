/* 
 * libdivecomputer
 * 
 * Copyright (C) 2008 Jef Driesen
 * 
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 * 
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 * 
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston,
 * MA 02110-1301 USA
 */

#include <stdlib.h>

#include "parser-private.h"


void
parser_init (dc_parser_t *parser, const parser_backend_t *backend)
{
	parser->backend = backend;
	parser->data = NULL;
	parser->size = 0;
}


dc_family_t
dc_parser_get_type (dc_parser_t *parser)
{
	if (parser == NULL)
		return DC_FAMILY_NULL;

	return parser->backend->type;
}


dc_status_t
dc_parser_set_data (dc_parser_t *parser, const unsigned char *data, unsigned int size)
{
	if (parser == NULL)
		return DC_STATUS_UNSUPPORTED;

	if (parser->backend->set_data == NULL)
		return DC_STATUS_UNSUPPORTED;

	parser->data = data;
	parser->size = size;

	return parser->backend->set_data (parser, data, size);
}


dc_status_t
dc_parser_get_datetime (dc_parser_t *parser, dc_datetime_t *datetime)
{
	if (parser == NULL)
		return DC_STATUS_UNSUPPORTED;

	if (parser->backend->datetime == NULL)
		return DC_STATUS_UNSUPPORTED;

	return parser->backend->datetime (parser, datetime);
}

dc_status_t
dc_parser_get_field (dc_parser_t *parser, dc_field_type_t type, unsigned int flags, void *value)
{
	if (parser == NULL)
		return DC_STATUS_UNSUPPORTED;

	if (parser->backend->field == NULL)
		return DC_STATUS_UNSUPPORTED;

	return parser->backend->field (parser, type, flags, value);
}


dc_status_t
dc_parser_samples_foreach (dc_parser_t *parser, dc_sample_callback_t callback, void *userdata)
{
	if (parser == NULL)
		return DC_STATUS_UNSUPPORTED;

	if (parser->backend->samples_foreach == NULL)
		return DC_STATUS_UNSUPPORTED;

	return parser->backend->samples_foreach (parser, callback, userdata);
}


dc_status_t
dc_parser_destroy (dc_parser_t *parser)
{
	if (parser == NULL)
		return DC_STATUS_SUCCESS;

	if (parser->backend->destroy == NULL)
		return DC_STATUS_UNSUPPORTED;

	return parser->backend->destroy (parser);
}


void
sample_statistics_cb (dc_sample_type_t type, dc_sample_value_t value, void *userdata)
{
	sample_statistics_t *statistics  = (sample_statistics_t *) userdata;

	switch (type) {
	case DC_SAMPLE_TIME:
		statistics->divetime = value.time;
		break;
	case DC_SAMPLE_DEPTH:
		if (statistics->maxdepth < value.depth)
			statistics->maxdepth = value.depth;
		break;
	default:
		break;
	}
}
