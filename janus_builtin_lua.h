/**
 * @file janus_builtin_lua.h
 * @author jessesleep@hotmail.com 
 * @date 2017/01/23 16:30:32
 * @brief 
 *  
 **/

#include <string>
#include <map>
#include <vector>

using namespace std;

#ifndef  __JANUS_LUA_SCRIPTS_H_
#define  __JANUS_LUA_SCRIPTS_H_

map<string, string> g_janus_builtin_lua_map;

const string g_janus_builtin_lua_names[] = {
    "janus_builtin_paxos_prepare.lua",
    "janus_builtin_paxos_accept.lua",
    "janus_builtin_renew.lua",
    "janus_builtin_check.lua",
    ""
};

const string g_janus_builtin_lua_content[] = {
    "if (5 ~= table.getn(ARGV)) then return {-1, 'invalid param nums'}; end; local lock_name = ARGV[1]; local instance_id = tonumber(ARGV[2]); local proposal_id_fir = tonumber(ARGV[3]); local proposal_id_sec = tonumber(ARGV[4]); local force = tonumber(ARGV[5]); local ret; local allowed_to_participate_switch; local outstanding_instance_id; local arr_instance_info; local proposal_id_highest_seen_fir; local proposal_id_highest_seen_sec; local proposal_id_highest_accepted_fir; local proposal_id_highest_accepted_sec; local accepted_value; local key_allowed_to_participate_switch = 'allowed_to_participate_switch_{' .. lock_name .. '}'; local key_outstanding_instance_id = 'outstanding_instance_id_{' .. lock_name .. '}'; local key_locks_instances_hash = 'locks_instances_hash_{' .. lock_name .. '}_{' .. instance_id .. '}'; local field_proposal_id_highest_seen_fir = 'proposal_id_highest_seen_fir'; local field_proposal_id_highest_seen_sec = 'proposal_id_highest_seen_sec'; local field_proposal_id_highest_accepted_fir = 'proposal_id_highest_accepted_fir'; local field_proposal_id_highest_accepted_sec = 'proposal_id_highest_accepted_sec'; local field_accepted_value = 'accepted_value'; local allowed_to_participate_switch = redis.call('GET', key_allowed_to_participate_switch); if (false == allowed_to_participate_switch) then if (1 == force) then redis.call('SET', key_allowed_to_participate_switch, 'allowed_to_participate_switch'); else return {3, 'NOT allowed_to_participate_switch'}; end; end; outstanding_instance_id = redis.call('GET', key_outstanding_instance_id); if (false == outstanding_instance_id) or (tonumber(outstanding_instance_id) < instance_id) then ret = redis.call('SET', key_outstanding_instance_id, instance_id); elseif (tonumber(outstanding_instance_id) > instance_id) then return {1, 'larger instance_id exists, update', outstanding_instance_id}; end; arr_instance_info = redis.call('HKEYS', key_locks_instances_hash); if (nil == next(arr_instance_info)) then ret = redis.call('HSET', key_locks_instances_hash, field_proposal_id_highest_seen_fir, proposal_id_fir); ret = redis.call('HSET', key_locks_instances_hash, field_proposal_id_highest_seen_sec, proposal_id_sec); end; proposal_id_highest_seen_fir = redis.call('HGET', key_locks_instances_hash, field_proposal_id_highest_seen_fir); if (false == proposal_id_highest_seen_fir) then return {-1, 'proposal_id_highest_seen_fir not exists'}; elseif (tonumber(proposal_id_highest_seen_fir) > proposal_id_fir) then return {2, 'reject due to higher proposal_id_fir'}; elseif (tonumber(proposal_id_highest_seen_fir) == proposal_id_fir) then proposal_id_highest_seen_sec = redis.call('HGET', key_locks_instances_hash, field_proposal_id_highest_seen_sec); if (false == proposal_id_highest_seen_sec) then return {-1, 'proposal_id_highest_seen_sec not exists'}; elseif (tonumber(proposal_id_highest_seen_sec) > proposal_id_sec) then return {2, 'reject due to higher proposal_id_sec'}; end; end; ret = redis.call('HSET', key_locks_instances_hash, field_proposal_id_highest_seen_fir, proposal_id_fir); ret = redis.call('HSET', key_locks_instances_hash, field_proposal_id_highest_seen_sec, proposal_id_sec); proposal_id_highest_accepted_fir = redis.call('HGET', key_locks_instances_hash, field_proposal_id_highest_accepted_fir); proposal_id_highest_accepted_sec = redis.call('HGET', key_locks_instances_hash, field_proposal_id_highest_accepted_sec); accepted_value = redis.call('HGET', key_locks_instances_hash, field_accepted_value); if (false == proposal_id_highest_accepted_fir) and (false == proposal_id_highest_accepted_sec) and (false == accepted_value) then return {0, 'success without accepted proposal', '', '', ''}; elseif (false == proposal_id_highest_accepted_fir) or (false == proposal_id_highest_accepted_sec) or (false == accepted_value) then return {-1 ,'missing info about accepted proposal'}; end; return {0, 'success with accepted proposal', tonumber(proposal_id_highest_accepted_fir), tonumber(proposal_id_highest_accepted_sec), accepted_value};",
    "if (5 ~= table.getn(ARGV)) then return {-1, 'invalid param nums'}; end; local lock_name = ARGV[1]; local instance_id = tonumber(ARGV[2]); local proposal_id_fir = tonumber(ARGV[3]); local proposal_id_sec = tonumber(ARGV[4]); local proposal_value = ARGV[5]; local ret; local allowed_to_participate_switch; local outstanding_instance_id; local arr_instance_info; local proposal_id_highest_seen_fir; local proposal_id_highest_seen_sec; local key_allowed_to_participate_switch = 'allowed_to_participate_switch_{' .. lock_name .. '}'; local key_outstanding_instance_id = 'outstanding_instance_id_{' .. lock_name .. '}'; local key_locks_instances_hash = 'locks_instances_hash_{' .. lock_name .. '}_{' .. instance_id .. '}'; local field_proposal_id_highest_seen_fir = 'proposal_id_highest_seen_fir'; local field_proposal_id_highest_seen_sec = 'proposal_id_highest_seen_sec'; local field_proposal_id_highest_accepted_fir = 'proposal_id_highest_accepted_fir'; local field_proposal_id_highest_accepted_sec = 'proposal_id_highest_accepted_sec'; local field_accepted_value = 'accepted_value'; local allowed_to_participate_switch = redis.call('GET', key_allowed_to_participate_switch); if (false == allowed_to_participate_switch) then return {3, 'NOT allowed_to_participate_switch'}; end; outstanding_instance_id = redis.call('GET', key_outstanding_instance_id); if (false == outstanding_instance_id) then return {2, 'reject due to non-existent outstanding_instance_id'}; elseif (tonumber(outstanding_instance_id) < instance_id) then return {2, 'reject due to lower outstanding_instance_id'}; elseif (tonumber(outstanding_instance_id) > instance_id) then return {1, 'larger instance_id exists, update', outstanding_instance_id}; end; arr_instance_info = redis.call('HKEYS', key_locks_instances_hash); if (nil == next(arr_instance_info)) then return {2, 'reject due to non-existent locks_instances_hash'}; end; proposal_id_highest_seen_fir = redis.call('HGET', key_locks_instances_hash, field_proposal_id_highest_seen_fir); if (false == proposal_id_highest_seen_fir) then return {-1, 'proposal_id_highest_seen_fir not exists'}; elseif (tonumber(proposal_id_highest_seen_fir) > proposal_id_fir) then return {2, 'reject due to higher proposal_id_fir'}; elseif (tonumber(proposal_id_highest_seen_fir) == proposal_id_fir) then proposal_id_highest_seen_sec = redis.call('HGET', key_locks_instances_hash, field_proposal_id_highest_seen_sec); if (false == proposal_id_highest_seen_sec) then return {-1, 'proposal_id_highest_seen_sec not exists'}; elseif (tonumber(proposal_id_highest_seen_sec) > proposal_id_sec) then return {2, 'reject due to higher proposal_id_sec'}; end; end; ret = redis.call('HSET', key_locks_instances_hash, field_proposal_id_highest_seen_fir, proposal_id_fir); ret = redis.call('HSET', key_locks_instances_hash, field_proposal_id_highest_seen_sec, proposal_id_sec); ret = redis.call('HSET', key_locks_instances_hash, field_proposal_id_highest_accepted_fir, proposal_id_fir); ret = redis.call('HSET', key_locks_instances_hash, field_proposal_id_highest_accepted_sec, proposal_id_sec); ret = redis.call('HSET', key_locks_instances_hash, field_accepted_value, proposal_value); return {0, 'success'};",
    "if (3 ~= table.getn(ARGV)) then return {-1, 'invalid param nums'}; end; local lock_name = ARGV[1]; local instance_id = tonumber(ARGV[2]); local own_value = ARGV[3]; local ret; local allowed_to_participate_switch; local outstanding_instance_id; local arr_instance_info; local accepted_value; local key_allowed_to_participate_switch = 'allowed_to_participate_switch_{' .. lock_name .. '}'; local key_outstanding_instance_id = 'outstanding_instance_id_{' .. lock_name .. '}'; local key_locks_instances_hash = 'locks_instances_hash_{' .. lock_name .. '}_{' .. instance_id .. '}'; local field_accepted_value = 'accepted_value'; local field_seq_num = 'seq_num'; local allowed_to_participate_switch = redis.call('GET', key_allowed_to_participate_switch); if (false == allowed_to_participate_switch) then return {3, 'NOT allowed_to_participate_switch'}; end; outstanding_instance_id = redis.call('GET', key_outstanding_instance_id); if (false == outstanding_instance_id) then return {2, 'reject due to non-existent outstanding_instance_id'}; elseif (tonumber(outstanding_instance_id) < instance_id) then return {2, 'reject due to lower outstanding_instance_id'}; elseif (tonumber(outstanding_instance_id) > instance_id) then return {1, 'larger instance_id exists, update', outstanding_instance_id}; end; arr_instance_info = redis.call('HKEYS', key_locks_instances_hash); if (nil == next(arr_instance_info)) then return {2, 'reject due to non-existent locks_instances_hash'}; end; accepted_value = redis.call('HGET', key_locks_instances_hash, field_accepted_value); if (false == accepted_value) then return {2, 'reject due to non-existent accepted_value'}; elseif (own_value ~= accepted_value) then return {2, 'reject due to different accepted_value'}; end; redis.call('HINCRBY', key_locks_instances_hash, field_seq_num, 1); return {0, 'success'};",
    "if (3 ~= table.getn(ARGV)) then return {-1, 'invalid param nums'}; end; local lock_name = ARGV[1]; local instance_id = tonumber(ARGV[2]); local last_seq_num = tonumber(ARGV[3]); local ret; local allowed_to_participate_switch; local outstanding_instance_id; local arr_instance_info; local seq_num; local key_allowed_to_participate_switch = 'allowed_to_participate_switch_{' .. lock_name .. '}'; local key_outstanding_instance_id = 'outstanding_instance_id_{' .. lock_name .. '}'; local key_locks_instances_hash = 'locks_instances_hash_{' .. lock_name .. '}_{' .. instance_id .. '}'; local field_seq_num = 'seq_num'; local allowed_to_participate_switch = redis.call('GET', key_allowed_to_participate_switch); if (false == allowed_to_participate_switch) then return {3, 'NOT allowed_to_participate_switch'}; end; outstanding_instance_id = redis.call('GET', key_outstanding_instance_id); if (false == outstanding_instance_id) then return {2, 'reject due to non-existent outstanding_instance_id'}; elseif (tonumber(outstanding_instance_id) < instance_id) then return {2, 'reject due to lower outstanding_instance_id'}; elseif (tonumber(outstanding_instance_id) > instance_id) then return {1, 'larger instance_id exists, update', outstanding_instance_id}; end; arr_instance_info = redis.call('HKEYS', key_locks_instances_hash); if (nil == next(arr_instance_info)) then return {2, 'reject due to non-existent locks_instances_hash'}; end; seq_num = redis.call('HGET', key_locks_instances_hash, field_seq_num); if (false == seq_num) then return {2, 'reject due to non-existent seq_num'}; elseif (tonumber(seq_num) == last_seq_num) or (tonumber(seq_num) < last_seq_num) then return {2, 'reject due to equal or smaller seq_num'}; end; return {0, 'success', seq_num};",
    ""
};

const string g_janus_builtin_lua_atomic_lockcheck = "local janus_builtin_avoid_repetition_argc = table.getn(ARGV);  if (janus_builtin_avoid_repetition_argc < 3) then  return {-1, 'janus_builtin_atomic_lockcheck_failed, too few args'};  end;  local janus_builtin_avoid_repetition_switch = ARGV[janus_builtin_avoid_repetition_argc - 2];  local janus_builtin_avoid_repetition_lock_name = ARGV[janus_builtin_avoid_repetition_argc - 1];  local janus_builtin_avoid_repetition_instance_id = tonumber(ARGV[janus_builtin_avoid_repetition_argc]);  if ('janus_builtin_atomic_lockcheck_yes' == janus_builtin_avoid_repetition_switch) then local janus_builtin_avoid_repetition_key_allowed_to_participate_switch = 'allowed_to_participate_switch_{' .. janus_builtin_avoid_repetition_lock_name .. '}'; local janus_builtin_avoid_repetition_allowed_to_participate_switch = redis.call('GET', janus_builtin_avoid_repetition_key_allowed_to_participate_switch); if (false == janus_builtin_avoid_repetition_allowed_to_participate_switch) then return {-1, 'janus_builtin_atomic_lockcheck_failed, NOT allowed_to_participate_switch'}; end; local janus_builtin_avoid_repetition_key_outstanding_instance_id = 'outstanding_instance_id_{' .. janus_builtin_avoid_repetition_lock_name .. '}'; local janus_builtin_avoid_repetition_outstanding_instance_id = redis.call('GET', janus_builtin_avoid_repetition_key_outstanding_instance_id); if (false == janus_builtin_avoid_repetition_outstanding_instance_id) then return {-1, 'janus_builtin_atomic_lockcheck_failed, outstanding_instance_id no exists'}; end; if (tonumber(janus_builtin_avoid_repetition_outstanding_instance_id) > janus_builtin_avoid_repetition_instance_id) then return {-1, 'janus_builtin_atomic_lockcheck_failed, outstanding_instance_id higher'}; end; redis.call('SET', janus_builtin_avoid_repetition_key_outstanding_instance_id, janus_builtin_avoid_repetition_instance_id);  end; ARGV[janus_builtin_avoid_repetition_argc - 2] = nil;  ARGV[janus_builtin_avoid_repetition_argc - 1] = nil;  ARGV[janus_builtin_avoid_repetition_argc] = nil;";

#endif  //__JANUS_LUA_SCRIPTS_H_

/* vim: set expandtab ts=4 sw=4 sts=4 tw=100: */
