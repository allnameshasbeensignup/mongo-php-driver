/*
 * Copyright 2014-2017 MongoDB, Inc.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *   http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include <php.h>
#include <Zend/zend_interfaces.h>

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include "phongo_compat.h"
#include "php_phongo.h"
#include "php_bson.h"

zend_class_entry* php_phongo_server_ce;

/* {{{ proto MongoDB\Driver\Cursor MongoDB\Driver\Server::executeCommand(string $db, MongoDB\Driver\Command $command[, array $options = null]))
   Executes a Command on this Server */
static PHP_METHOD(Server, executeCommand)
{
	zend_error_handling  error_handling;
	php_phongo_server_t* intern;
	char*                db;
	size_t               db_len;
	zval*                command;
	zval*                options      = NULL;
	bool                 free_options = false;

	intern = Z_SERVER_OBJ_P(getThis());

	zend_replace_error_handling(EH_THROW, phongo_exception_from_phongo_domain(PHONGO_ERROR_INVALID_ARGUMENT), &error_handling);
	if (zend_parse_parameters(ZEND_NUM_ARGS(), "sO|z!", &db, &db_len, &command, php_phongo_command_ce, &options) == FAILURE) {
		zend_restore_error_handling(&error_handling);
		return;
	}
	zend_restore_error_handling(&error_handling);

	options = php_phongo_prep_legacy_option(options, "readPreference", &free_options);

	/* If the Server was created in a different process, reset the client so
	 * that cursors created by this process can be differentiated and its
	 * session pool is cleared. */
	PHONGO_RESET_CLIENT_IF_PID_DIFFERS(intern);

	phongo_execute_command(intern->client, PHONGO_COMMAND_RAW, db, command, options, intern->server_id, return_value);

	if (free_options) {
		php_phongo_prep_legacy_option_free(options);
	}
} /* }}} */

/* {{{ proto MongoDB\Driver\Cursor MongoDB\Driver\Server::executeReadCommand(string $db, MongoDB\Driver\Command $command[, array $options = null]))
   Executes a ReadCommand on this Server */
static PHP_METHOD(Server, executeReadCommand)
{
	zend_error_handling  error_handling;
	php_phongo_server_t* intern;
	char*                db;
	size_t               db_len;
	zval*                command;
	zval*                options = NULL;

	intern = Z_SERVER_OBJ_P(getThis());

	zend_replace_error_handling(EH_THROW, phongo_exception_from_phongo_domain(PHONGO_ERROR_INVALID_ARGUMENT), &error_handling);
	if (zend_parse_parameters(ZEND_NUM_ARGS(), "sO|a!", &db, &db_len, &command, php_phongo_command_ce, &options) == FAILURE) {
		zend_restore_error_handling(&error_handling);
		return;
	}
	zend_restore_error_handling(&error_handling);

	/* If the Server was created in a different process, reset the client so
	 * that cursors created by this process can be differentiated and its
	 * session pool is cleared. */
	PHONGO_RESET_CLIENT_IF_PID_DIFFERS(intern);

	phongo_execute_command(intern->client, PHONGO_COMMAND_READ, db, command, options, intern->server_id, return_value);
} /* }}} */

/* {{{ proto MongoDB\Driver\Cursor MongoDB\Driver\Server::executeWriteCommand(string $db, MongoDB\Driver\Command $command[, array $options = null]))
   Executes a WriteCommand on this Server */
static PHP_METHOD(Server, executeWriteCommand)
{
	zend_error_handling  error_handling;
	php_phongo_server_t* intern;
	char*                db;
	size_t               db_len;
	zval*                command;
	zval*                options = NULL;

	intern = Z_SERVER_OBJ_P(getThis());

	zend_replace_error_handling(EH_THROW, phongo_exception_from_phongo_domain(PHONGO_ERROR_INVALID_ARGUMENT), &error_handling);
	if (zend_parse_parameters(ZEND_NUM_ARGS(), "sO|a!", &db, &db_len, &command, php_phongo_command_ce, &options) == FAILURE) {
		zend_restore_error_handling(&error_handling);
		return;
	}
	zend_restore_error_handling(&error_handling);

	/* If the Server was created in a different process, reset the client so
	 * that cursors created by this process can be differentiated. and its
	 * session pool is cleared. */
	PHONGO_RESET_CLIENT_IF_PID_DIFFERS(intern);

	phongo_execute_command(intern->client, PHONGO_COMMAND_WRITE, db, command, options, intern->server_id, return_value);
} /* }}} */

/* {{{ proto MongoDB\Driver\Cursor MongoDB\Driver\Server::executeReadWriteCommand(string $db, MongoDB\Driver\Command $command[, array $options = null]))
   Executes a ReadWriteCommand on this Server */
static PHP_METHOD(Server, executeReadWriteCommand)
{
	zend_error_handling  error_handling;
	php_phongo_server_t* intern;
	char*                db;
	size_t               db_len;
	zval*                command;
	zval*                options = NULL;

	intern = Z_SERVER_OBJ_P(getThis());

	zend_replace_error_handling(EH_THROW, phongo_exception_from_phongo_domain(PHONGO_ERROR_INVALID_ARGUMENT), &error_handling);
	if (zend_parse_parameters(ZEND_NUM_ARGS(), "sO|a!", &db, &db_len, &command, php_phongo_command_ce, &options) == FAILURE) {
		zend_restore_error_handling(&error_handling);
		return;
	}
	zend_restore_error_handling(&error_handling);

	/* If the Server was created in a different process, reset the client so
	 * that cursors created by this process can be differentiated and its
	 * session pool is cleared. */
	PHONGO_RESET_CLIENT_IF_PID_DIFFERS(intern);

	phongo_execute_command(intern->client, PHONGO_COMMAND_READ_WRITE, db, command, options, intern->server_id, return_value);
} /* }}} */

/* {{{ proto MongoDB\Driver\Cursor MongoDB\Driver\Server::executeQuery(string $namespace, MongoDB\Driver\Query $query[, array $options = null]))
   Executes a Query on this Server */
static PHP_METHOD(Server, executeQuery)
{
	zend_error_handling  error_handling;
	php_phongo_server_t* intern;
	char* namespace;
	size_t namespace_len;
	zval*  query;
	zval*  options      = NULL;
	bool   free_options = false;

	intern = Z_SERVER_OBJ_P(getThis());

	zend_replace_error_handling(EH_THROW, phongo_exception_from_phongo_domain(PHONGO_ERROR_INVALID_ARGUMENT), &error_handling);
	if (zend_parse_parameters(ZEND_NUM_ARGS(), "sO|z!", &namespace, &namespace_len, &query, php_phongo_query_ce, &options) == FAILURE) {
		zend_restore_error_handling(&error_handling);
		return;
	}
	zend_restore_error_handling(&error_handling);

	options = php_phongo_prep_legacy_option(options, "readPreference", &free_options);

	/* If the Server was created in a different process, reset the client so
	 * that cursors created by this process can be differentiated and its
	 * session pool is cleared. */
	PHONGO_RESET_CLIENT_IF_PID_DIFFERS(intern);

	phongo_execute_query(intern->client, namespace, query, options, intern->server_id, return_value);

	if (free_options) {
		php_phongo_prep_legacy_option_free(options);
	}
} /* }}} */

/* {{{ proto MongoDB\Driver\WriteResult MongoDB\Driver\Server::executeBulkWrite(string $namespace, MongoDB\Driver\BulkWrite $zbulk[, array $options = null])
   Executes a BulkWrite (i.e. any number of insert, update, and delete ops) on
   this Server */
static PHP_METHOD(Server, executeBulkWrite)
{
	zend_error_handling  error_handling;
	php_phongo_server_t* intern;
	char* namespace;
	size_t                  namespace_len;
	zval*                   zbulk;
	php_phongo_bulkwrite_t* bulk;
	zval*                   options      = NULL;
	bool                    free_options = false;

	intern = Z_SERVER_OBJ_P(getThis());

	zend_replace_error_handling(EH_THROW, phongo_exception_from_phongo_domain(PHONGO_ERROR_INVALID_ARGUMENT), &error_handling);
	if (zend_parse_parameters(ZEND_NUM_ARGS(), "sO|z!", &namespace, &namespace_len, &zbulk, php_phongo_bulkwrite_ce, &options, php_phongo_writeconcern_ce) == FAILURE) {
		zend_restore_error_handling(&error_handling);
		return;
	}
	zend_restore_error_handling(&error_handling);

	bulk = Z_BULKWRITE_OBJ_P(zbulk);

	options = php_phongo_prep_legacy_option(options, "writeConcern", &free_options);

	/* If the Server was created in a different process, reset the client so
	 * that its session pool is cleared. */
	PHONGO_RESET_CLIENT_IF_PID_DIFFERS(intern);

	phongo_execute_bulk_write(intern->client, namespace, bulk, options, intern->server_id, return_value);

	if (free_options) {
		php_phongo_prep_legacy_option_free(options);
	}
} /* }}} */

/* {{{ proto string MongoDB\Driver\Server::getHost()
   Returns the hostname for this Server */
static PHP_METHOD(Server, getHost)
{
	zend_error_handling          error_handling;
	php_phongo_server_t*         intern;
	mongoc_server_description_t* sd;

	intern = Z_SERVER_OBJ_P(getThis());

	zend_replace_error_handling(EH_THROW, phongo_exception_from_phongo_domain(PHONGO_ERROR_INVALID_ARGUMENT), &error_handling);
	if (zend_parse_parameters_none() == FAILURE) {
		zend_restore_error_handling(&error_handling);
		return;
	}
	zend_restore_error_handling(&error_handling);

	if ((sd = mongoc_client_get_server_description(intern->client, intern->server_id))) {
		RETVAL_STRING(mongoc_server_description_host(sd)->host);
		mongoc_server_description_destroy(sd);
		return;
	}

	phongo_throw_exception(PHONGO_ERROR_RUNTIME, "Failed to get server description");
} /* }}} */

/* {{{ proto array MongoDB\Driver\Server::getTags()
   Returns the currently configured tags for this Server */
static PHP_METHOD(Server, getTags)
{
	zend_error_handling          error_handling;
	php_phongo_server_t*         intern;
	mongoc_server_description_t* sd;

	intern = Z_SERVER_OBJ_P(getThis());

	zend_replace_error_handling(EH_THROW, phongo_exception_from_phongo_domain(PHONGO_ERROR_INVALID_ARGUMENT), &error_handling);
	if (zend_parse_parameters_none() == FAILURE) {
		zend_restore_error_handling(&error_handling);
		return;
	}
	zend_restore_error_handling(&error_handling);

	if ((sd = mongoc_client_get_server_description(intern->client, intern->server_id))) {
		const bson_t* is_master = mongoc_server_description_ismaster(sd);
		bson_iter_t   iter;

		if (bson_iter_init_find(&iter, is_master, "tags") && BSON_ITER_HOLDS_DOCUMENT(&iter)) {
			const uint8_t*        bytes;
			uint32_t              len;
			php_phongo_bson_state state;

			PHONGO_BSON_INIT_DEBUG_STATE(state);
			bson_iter_document(&iter, &len, &bytes);

			if (!php_phongo_bson_to_zval_ex(bytes, len, &state)) {
				/* Exception should already have been thrown */
				zval_ptr_dtor(&state.zchild);
				mongoc_server_description_destroy(sd);
				return;
			}

			mongoc_server_description_destroy(sd);

			RETURN_ZVAL(&state.zchild, 0, 1);
		}

		array_init(return_value);
		mongoc_server_description_destroy(sd);
		return;
	}

	phongo_throw_exception(PHONGO_ERROR_RUNTIME, "Failed to get server description");
} /* }}} */

/* {{{ proto array MongoDB\Driver\Server::getInfo()
   Returns the last isMaster result document for this Server */
static PHP_METHOD(Server, getInfo)
{
	zend_error_handling          error_handling;
	php_phongo_server_t*         intern;
	mongoc_server_description_t* sd;

	intern = Z_SERVER_OBJ_P(getThis());

	zend_replace_error_handling(EH_THROW, phongo_exception_from_phongo_domain(PHONGO_ERROR_INVALID_ARGUMENT), &error_handling);
	if (zend_parse_parameters_none() == FAILURE) {
		zend_restore_error_handling(&error_handling);
		return;
	}
	zend_restore_error_handling(&error_handling);

	if ((sd = mongoc_client_get_server_description(intern->client, intern->server_id))) {
		const bson_t*         is_master = mongoc_server_description_ismaster(sd);
		php_phongo_bson_state state;

		PHONGO_BSON_INIT_DEBUG_STATE(state);

		if (!php_phongo_bson_to_zval_ex(bson_get_data(is_master), is_master->len, &state)) {
			/* Exception should already have been thrown */
			zval_ptr_dtor(&state.zchild);
			mongoc_server_description_destroy(sd);
			return;
		}

		mongoc_server_description_destroy(sd);

		RETURN_ZVAL(&state.zchild, 0, 1);
	}

	phongo_throw_exception(PHONGO_ERROR_RUNTIME, "Failed to get server description");
} /* }}} */

/* {{{ proto integer MongoDB\Driver\Server::getLatency()
   Returns the last measured latency for this Server */
static PHP_METHOD(Server, getLatency)
{
	zend_error_handling          error_handling;
	php_phongo_server_t*         intern;
	mongoc_server_description_t* sd;

	intern = Z_SERVER_OBJ_P(getThis());

	zend_replace_error_handling(EH_THROW, phongo_exception_from_phongo_domain(PHONGO_ERROR_INVALID_ARGUMENT), &error_handling);
	if (zend_parse_parameters_none() == FAILURE) {
		zend_restore_error_handling(&error_handling);
		return;
	}
	zend_restore_error_handling(&error_handling);

	if ((sd = mongoc_client_get_server_description(intern->client, intern->server_id))) {
		RETVAL_LONG((zend_long) mongoc_server_description_round_trip_time(sd));
		mongoc_server_description_destroy(sd);
		return;
	}

	phongo_throw_exception(PHONGO_ERROR_RUNTIME, "Failed to get server description");
} /* }}} */

/* {{{ proto integer MongoDB\Driver\Server::getPort()
   Returns the port for this Server */
static PHP_METHOD(Server, getPort)
{
	zend_error_handling          error_handling;
	php_phongo_server_t*         intern;
	mongoc_server_description_t* sd;

	intern = Z_SERVER_OBJ_P(getThis());

	zend_replace_error_handling(EH_THROW, phongo_exception_from_phongo_domain(PHONGO_ERROR_INVALID_ARGUMENT), &error_handling);
	if (zend_parse_parameters_none() == FAILURE) {
		zend_restore_error_handling(&error_handling);
		return;
	}
	zend_restore_error_handling(&error_handling);

	if ((sd = mongoc_client_get_server_description(intern->client, intern->server_id))) {
		RETVAL_LONG(mongoc_server_description_host(sd)->port);
		mongoc_server_description_destroy(sd);
		return;
	}

	phongo_throw_exception(PHONGO_ERROR_RUNTIME, "Failed to get server description");
} /* }}} */

/* {{{ proto integer MongoDB\Driver\Server::getType()
   Returns the node type of this Server */
static PHP_METHOD(Server, getType)
{
	zend_error_handling          error_handling;
	php_phongo_server_t*         intern;
	mongoc_server_description_t* sd;

	intern = Z_SERVER_OBJ_P(getThis());

	zend_replace_error_handling(EH_THROW, phongo_exception_from_phongo_domain(PHONGO_ERROR_INVALID_ARGUMENT), &error_handling);
	if (zend_parse_parameters_none() == FAILURE) {
		zend_restore_error_handling(&error_handling);
		return;
	}
	zend_restore_error_handling(&error_handling);

	if ((sd = mongoc_client_get_server_description(intern->client, intern->server_id))) {
		RETVAL_LONG(php_phongo_server_description_type(sd));
		mongoc_server_description_destroy(sd);
		return;
	}

	phongo_throw_exception(PHONGO_ERROR_RUNTIME, "Failed to get server description");
} /* }}} */

/* {{{ proto boolean MongoDB\Driver\Server::isPrimary()
   Returns whether this Server is a primary member of a replica set */
static PHP_METHOD(Server, isPrimary)
{
	zend_error_handling          error_handling;
	php_phongo_server_t*         intern;
	mongoc_server_description_t* sd;

	intern = Z_SERVER_OBJ_P(getThis());

	zend_replace_error_handling(EH_THROW, phongo_exception_from_phongo_domain(PHONGO_ERROR_INVALID_ARGUMENT), &error_handling);
	if (zend_parse_parameters_none() == FAILURE) {
		zend_restore_error_handling(&error_handling);
		return;
	}
	zend_restore_error_handling(&error_handling);

	if ((sd = mongoc_client_get_server_description(intern->client, intern->server_id))) {
		RETVAL_BOOL(!strcmp(mongoc_server_description_type(sd), php_phongo_server_description_type_map[PHONGO_SERVER_RS_PRIMARY].name));
		mongoc_server_description_destroy(sd);
		return;
	}

	phongo_throw_exception(PHONGO_ERROR_RUNTIME, "Failed to get server description");
} /* }}} */

/* {{{ proto boolean MongoDB\Driver\Server::isSecondary()
   Returns whether this Server is a secondary member of a replica set */
static PHP_METHOD(Server, isSecondary)
{
	zend_error_handling          error_handling;
	php_phongo_server_t*         intern;
	mongoc_server_description_t* sd;

	intern = Z_SERVER_OBJ_P(getThis());

	zend_replace_error_handling(EH_THROW, phongo_exception_from_phongo_domain(PHONGO_ERROR_INVALID_ARGUMENT), &error_handling);
	if (zend_parse_parameters_none() == FAILURE) {
		zend_restore_error_handling(&error_handling);
		return;
	}
	zend_restore_error_handling(&error_handling);

	if ((sd = mongoc_client_get_server_description(intern->client, intern->server_id))) {
		RETVAL_BOOL(!strcmp(mongoc_server_description_type(sd), php_phongo_server_description_type_map[PHONGO_SERVER_RS_SECONDARY].name));
		mongoc_server_description_destroy(sd);
		return;
	}

	phongo_throw_exception(PHONGO_ERROR_RUNTIME, "Failed to get server description");
} /* }}} */

/* {{{ proto boolean MongoDB\Driver\Server::isArbiter()
   Returns whether this Server is an arbiter member of a replica set */
static PHP_METHOD(Server, isArbiter)
{
	zend_error_handling          error_handling;
	php_phongo_server_t*         intern;
	mongoc_server_description_t* sd;

	intern = Z_SERVER_OBJ_P(getThis());

	zend_replace_error_handling(EH_THROW, phongo_exception_from_phongo_domain(PHONGO_ERROR_INVALID_ARGUMENT), &error_handling);
	if (zend_parse_parameters_none() == FAILURE) {
		zend_restore_error_handling(&error_handling);
		return;
	}
	zend_restore_error_handling(&error_handling);

	if ((sd = mongoc_client_get_server_description(intern->client, intern->server_id))) {
		RETVAL_BOOL(!strcmp(mongoc_server_description_type(sd), php_phongo_server_description_type_map[PHONGO_SERVER_RS_ARBITER].name));
		mongoc_server_description_destroy(sd);
		return;
	}

	phongo_throw_exception(PHONGO_ERROR_RUNTIME, "Failed to get server description");
} /* }}} */

/* {{{ proto boolean MongoDB\Driver\Server::isHidden()
   Returns whether this Server is a hidden member of a replica set */
static PHP_METHOD(Server, isHidden)
{
	zend_error_handling          error_handling;
	php_phongo_server_t*         intern;
	mongoc_server_description_t* sd;

	intern = Z_SERVER_OBJ_P(getThis());

	zend_replace_error_handling(EH_THROW, phongo_exception_from_phongo_domain(PHONGO_ERROR_INVALID_ARGUMENT), &error_handling);
	if (zend_parse_parameters_none() == FAILURE) {
		zend_restore_error_handling(&error_handling);
		return;
	}
	zend_restore_error_handling(&error_handling);

	if ((sd = mongoc_client_get_server_description(intern->client, intern->server_id))) {
		bson_iter_t iter;

		RETVAL_BOOL(bson_iter_init_find_case(&iter, mongoc_server_description_ismaster(sd), "hidden") && bson_iter_as_bool(&iter));
		mongoc_server_description_destroy(sd);
		return;
	}

	phongo_throw_exception(PHONGO_ERROR_RUNTIME, "Failed to get server description");
} /* }}} */

/* {{{ proto boolean MongoDB\Driver\Server::isPassive()
   Returns whether this Server is a passive member of a replica set */
static PHP_METHOD(Server, isPassive)
{
	zend_error_handling          error_handling;
	php_phongo_server_t*         intern;
	mongoc_server_description_t* sd;

	intern = Z_SERVER_OBJ_P(getThis());

	zend_replace_error_handling(EH_THROW, phongo_exception_from_phongo_domain(PHONGO_ERROR_INVALID_ARGUMENT), &error_handling);
	if (zend_parse_parameters_none() == FAILURE) {
		zend_restore_error_handling(&error_handling);
		return;
	}
	zend_restore_error_handling(&error_handling);

	if ((sd = mongoc_client_get_server_description(intern->client, intern->server_id))) {
		bson_iter_t iter;

		RETVAL_BOOL(bson_iter_init_find_case(&iter, mongoc_server_description_ismaster(sd), "passive") && bson_iter_as_bool(&iter));
		mongoc_server_description_destroy(sd);
		return;
	}

	phongo_throw_exception(PHONGO_ERROR_RUNTIME, "Failed to get server description");
} /* }}} */

/* {{{ MongoDB\Driver\Server function entries */
ZEND_BEGIN_ARG_INFO_EX(ai_Server_executeCommand, 0, 0, 2)
	ZEND_ARG_INFO(0, db)
	ZEND_ARG_OBJ_INFO(0, command, MongoDB\\Driver\\Command, 0)
	ZEND_ARG_INFO(0, options)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(ai_Server_executeRWCommand, 0, 0, 2)
	ZEND_ARG_INFO(0, db)
	ZEND_ARG_OBJ_INFO(0, command, MongoDB\\Driver\\Command, 0)
	ZEND_ARG_ARRAY_INFO(0, options, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(ai_Server_executeQuery, 0, 0, 2)
	ZEND_ARG_INFO(0, namespace)
	ZEND_ARG_OBJ_INFO(0, zquery, MongoDB\\Driver\\Query, 0)
	ZEND_ARG_INFO(0, options)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(ai_Server_executeBulkWrite, 0, 0, 2)
	ZEND_ARG_INFO(0, namespace)
	ZEND_ARG_OBJ_INFO(0, zbulk, MongoDB\\Driver\\BulkWrite, 0)
	ZEND_ARG_INFO(0, options)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(ai_Server_void, 0, 0, 0)
ZEND_END_ARG_INFO()

static zend_function_entry php_phongo_server_me[] = {
	/* clang-format off */
	PHP_ME(Server, executeCommand, ai_Server_executeCommand, ZEND_ACC_PUBLIC | ZEND_ACC_FINAL)
	PHP_ME(Server, executeReadCommand, ai_Server_executeRWCommand, ZEND_ACC_PUBLIC | ZEND_ACC_FINAL)
	PHP_ME(Server, executeWriteCommand, ai_Server_executeRWCommand, ZEND_ACC_PUBLIC | ZEND_ACC_FINAL)
	PHP_ME(Server, executeReadWriteCommand, ai_Server_executeRWCommand, ZEND_ACC_PUBLIC | ZEND_ACC_FINAL)
	PHP_ME(Server, executeQuery, ai_Server_executeQuery, ZEND_ACC_PUBLIC | ZEND_ACC_FINAL)
	PHP_ME(Server, executeBulkWrite, ai_Server_executeBulkWrite, ZEND_ACC_PUBLIC | ZEND_ACC_FINAL)
	PHP_ME(Server, getHost, ai_Server_void, ZEND_ACC_PUBLIC | ZEND_ACC_FINAL)
	PHP_ME(Server, getTags, ai_Server_void, ZEND_ACC_PUBLIC | ZEND_ACC_FINAL)
	PHP_ME(Server, getInfo, ai_Server_void, ZEND_ACC_PUBLIC | ZEND_ACC_FINAL)
	PHP_ME(Server, getLatency, ai_Server_void, ZEND_ACC_PUBLIC | ZEND_ACC_FINAL)
	PHP_ME(Server, getPort, ai_Server_void, ZEND_ACC_PUBLIC | ZEND_ACC_FINAL)
	PHP_ME(Server, getType, ai_Server_void, ZEND_ACC_PUBLIC | ZEND_ACC_FINAL)
	PHP_ME(Server, isPrimary, ai_Server_void, ZEND_ACC_PUBLIC | ZEND_ACC_FINAL)
	PHP_ME(Server, isSecondary, ai_Server_void, ZEND_ACC_PUBLIC | ZEND_ACC_FINAL)
	PHP_ME(Server, isArbiter, ai_Server_void, ZEND_ACC_PUBLIC | ZEND_ACC_FINAL)
	PHP_ME(Server, isHidden, ai_Server_void, ZEND_ACC_PUBLIC | ZEND_ACC_FINAL)
	PHP_ME(Server, isPassive, ai_Server_void, ZEND_ACC_PUBLIC | ZEND_ACC_FINAL)
	ZEND_NAMED_ME(__construct, PHP_FN(MongoDB_disabled___construct), ai_Server_void, ZEND_ACC_PRIVATE | ZEND_ACC_FINAL)
	ZEND_NAMED_ME(__wakeup, PHP_FN(MongoDB_disabled___wakeup), ai_Server_void, ZEND_ACC_PUBLIC | ZEND_ACC_FINAL)
	PHP_FE_END
	/* clang-format on */
};
/* }}} */

/* {{{ MongoDB\Driver\Server object handlers */
static zend_object_handlers php_phongo_handler_server;

static int php_phongo_server_compare_objects(zval* o1, zval* o2) /* {{{ */
{
	php_phongo_server_t*         intern1;
	php_phongo_server_t*         intern2;
	mongoc_server_description_t *sd1, *sd2;
	int                          retval = 0;

	ZEND_COMPARE_OBJECTS_FALLBACK(o1, o2);

	intern1 = Z_SERVER_OBJ_P(o1);
	intern2 = Z_SERVER_OBJ_P(o2);

	sd1 = mongoc_client_get_server_description(intern1->client, intern1->server_id);
	sd2 = mongoc_client_get_server_description(intern2->client, intern2->server_id);

	if (sd1 && sd2) {
		retval = strcasecmp(mongoc_server_description_host(sd1)->host_and_port, mongoc_server_description_host(sd2)->host_and_port);
	} else {
		phongo_throw_exception(PHONGO_ERROR_RUNTIME, "Failed to get server description(s)");
	}

	if (sd1) {
		mongoc_server_description_destroy(sd1);
	}

	if (sd2) {
		mongoc_server_description_destroy(sd2);
	}

	return retval;
} /* }}} */

static void php_phongo_server_free_object(zend_object* object) /* {{{ */
{
	php_phongo_server_t* intern = Z_OBJ_SERVER(object);

	zend_object_std_dtor(&intern->std);
} /* }}} */

static zend_object* php_phongo_server_create_object(zend_class_entry* class_type) /* {{{ */
{
	php_phongo_server_t* intern = NULL;

	intern = PHONGO_ALLOC_OBJECT_T(php_phongo_server_t, class_type);

	zend_object_std_init(&intern->std, class_type);
	object_properties_init(&intern->std, class_type);

	PHONGO_SET_CREATED_BY_PID(intern);

	intern->std.handlers = &php_phongo_handler_server;

	return &intern->std;
} /* }}} */

static HashTable* php_phongo_server_get_debug_info(phongo_compat_object_handler_type* object, int* is_temp) /* {{{ */
{
	php_phongo_server_t*         intern = NULL;
	zval                         retval = ZVAL_STATIC_INIT;
	mongoc_server_description_t* sd;

	*is_temp = 1;
	intern   = Z_OBJ_SERVER(PHONGO_COMPAT_GET_OBJ(object));

	if (!(sd = mongoc_client_get_server_description(intern->client, intern->server_id))) {
		phongo_throw_exception(PHONGO_ERROR_RUNTIME, "Failed to get server description");
		return NULL;
	}

	php_phongo_server_to_zval(&retval, sd);
	mongoc_server_description_destroy(sd);

	return Z_ARRVAL(retval);
} /* }}} */
/* }}} */

void php_phongo_server_init_ce(INIT_FUNC_ARGS) /* {{{ */
{
	zend_class_entry ce;

	INIT_NS_CLASS_ENTRY(ce, "MongoDB\\Driver", "Server", php_phongo_server_me);
	php_phongo_server_ce                = zend_register_internal_class(&ce);
	php_phongo_server_ce->create_object = php_phongo_server_create_object;
	PHONGO_CE_FINAL(php_phongo_server_ce);
	PHONGO_CE_DISABLE_SERIALIZATION(php_phongo_server_ce);

	memcpy(&php_phongo_handler_server, phongo_get_std_object_handlers(), sizeof(zend_object_handlers));
	PHONGO_COMPAT_SET_COMPARE_OBJECTS_HANDLER(server);
	php_phongo_handler_server.get_debug_info = php_phongo_server_get_debug_info;
	php_phongo_handler_server.free_obj       = php_phongo_server_free_object;
	php_phongo_handler_server.offset         = XtOffsetOf(php_phongo_server_t, std);

	zend_declare_class_constant_long(php_phongo_server_ce, ZEND_STRL("TYPE_UNKNOWN"), PHONGO_SERVER_UNKNOWN);
	zend_declare_class_constant_long(php_phongo_server_ce, ZEND_STRL("TYPE_STANDALONE"), PHONGO_SERVER_STANDALONE);
	zend_declare_class_constant_long(php_phongo_server_ce, ZEND_STRL("TYPE_MONGOS"), PHONGO_SERVER_MONGOS);
	zend_declare_class_constant_long(php_phongo_server_ce, ZEND_STRL("TYPE_POSSIBLE_PRIMARY"), PHONGO_SERVER_POSSIBLE_PRIMARY);
	zend_declare_class_constant_long(php_phongo_server_ce, ZEND_STRL("TYPE_RS_PRIMARY"), PHONGO_SERVER_RS_PRIMARY);
	zend_declare_class_constant_long(php_phongo_server_ce, ZEND_STRL("TYPE_RS_SECONDARY"), PHONGO_SERVER_RS_SECONDARY);
	zend_declare_class_constant_long(php_phongo_server_ce, ZEND_STRL("TYPE_RS_ARBITER"), PHONGO_SERVER_RS_ARBITER);
	zend_declare_class_constant_long(php_phongo_server_ce, ZEND_STRL("TYPE_RS_OTHER"), PHONGO_SERVER_RS_OTHER);
	zend_declare_class_constant_long(php_phongo_server_ce, ZEND_STRL("TYPE_RS_GHOST"), PHONGO_SERVER_RS_GHOST);
} /* }}} */

/*
 * Local variables:
 * tab-width: 4
 * c-basic-offset: 4
 * End:
 * vim600: noet sw=4 ts=4 fdm=marker
 * vim<600: noet sw=4 ts=4
 */
