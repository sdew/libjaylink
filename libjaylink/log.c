/*
 * This file is part of the libjaylink project.
 *
 * Copyright (C) 2014-2015 Marc Schink <jaylink-dev@marcschink.de>
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include <stdio.h>
#include <stdarg.h>

#include "libjaylink.h"
#include "libjaylink-internal.h"

/**
 * @file
 *
 * Logging functions.
 */

/**
 * Set the libjaylink log level.
 *
 * @param[in,out] ctx libjaylink context.
 * @param[in] level Log level to set. See #jaylink_log_level for valid values.
 *
 * @retval JAYLINK_OK Success.
 * @retval JAYLINK_ERR_ARG Invalid arguments.
 */
JAYLINK_API int jaylink_log_set_level(struct jaylink_context *ctx, int level)
{
	if (!ctx)
		return JAYLINK_ERR_ARG;

	if (level < JAYLINK_LOG_LEVEL_NONE || level > JAYLINK_LOG_LEVEL_DEBUG)
		return JAYLINK_ERR_ARG;

	ctx->log_level = level;

	return JAYLINK_OK;
}

/**
 * Get the libjaylink log level.
 *
 * @param[in] ctx libjaylink context.
 *
 * @return The current log level on success, or a negative error code
 * 	   on failure. See #jaylink_log_level for a description of each
 * 	   individual log level.
 */
JAYLINK_API int jaylink_log_get_level(const struct jaylink_context *ctx)
{
	if (!ctx)
		return JAYLINK_ERR_ARG;

	return ctx->log_level;
}

/**
 * Set the libjaylink log callback function.
 *
 * @param[in,out] ctx libjaylink context.
 * @param[in] callback Callback function to use, or NULL to use the default log
 * 		       function.
 * @param[in] user_data User data to be passed to the callback function.
 *
 * @retval JAYLINK_OK Success.
 * @retval JAYLINK_ERR_ARG Invalid arguments.
 */
JAYLINK_API int jaylink_log_set_callback(struct jaylink_context *ctx,
		jaylink_log_callback callback, void *user_data)
{
	if (!ctx)
		return JAYLINK_ERR_ARG;

	if (callback) {
		ctx->log_callback = callback;
		ctx->log_callback_data = user_data;
	} else {
		ctx->log_callback = &log_vprintf;
		ctx->log_callback_data = NULL;
	}

	return JAYLINK_OK;
}

/** @private */
JAYLINK_PRIV int log_vprintf(const struct jaylink_context *ctx, int level,
		const char *format, va_list args, void *user_data)
{
	(void)user_data;

	/*
	 * Filter out messages with higher verbosity than the verbosity of the
	 * current log level.
	 */
	if (level > ctx->log_level)
		return 0;

	fprintf(stderr, "jaylink: ");
	vfprintf(stderr, format, args);
	fprintf(stderr, "\n");

	return 0;
}

/** @private */
JAYLINK_PRIV void log_err(const struct jaylink_context *ctx,
		const char *format, ...)
{
	va_list args;

	if (!ctx)
		return;

	va_start(args, format);
	ctx->log_callback(ctx, JAYLINK_LOG_LEVEL_ERROR, format, args,
		ctx->log_callback_data);
	va_end(args);
}

/** @private */
JAYLINK_PRIV void log_warn(const struct jaylink_context *ctx,
		const char *format, ...)
{
	va_list args;

	if (!ctx)
		return;

	va_start(args, format);
	ctx->log_callback(ctx, JAYLINK_LOG_LEVEL_WARNING, format, args,
		ctx->log_callback_data);
	va_end(args);
}

/** @private */
JAYLINK_PRIV void log_info(const struct jaylink_context *ctx,
		const char *format, ...)
{
	va_list args;

	if (!ctx)
		return;

	va_start(args, format);
	ctx->log_callback(ctx, JAYLINK_LOG_LEVEL_INFO, format, args,
		ctx->log_callback_data);
	va_end(args);
}

/** @private */
JAYLINK_PRIV void log_dbg(const struct jaylink_context *ctx,
		const char *format, ...)
{
	va_list args;

	if (!ctx)
		return;

	va_start(args, format);
	ctx->log_callback(ctx, JAYLINK_LOG_LEVEL_DEBUG, format, args,
		ctx->log_callback_data);
	va_end(args);
}
