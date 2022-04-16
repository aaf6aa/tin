#pragma once

#pragma GCC diagnostic ignored "-Wunused-parameter"
#pragma GCC diagnostic ignored "-Wuninitialized"
#pragma GCC diagnostic ignored "-Warray-bounds"
#pragma GCC diagnostic ignored "-Wreturn-type"
#pragma GCC diagnostic ignored "-Wsign-compare"
#pragma GCC diagnostic ignored "-Wunused-variable"
#pragma GCC diagnostic ignored "-Wswitch"
#pragma GCC diagnostic ignored "-Wpedantic"

#include "ast.h"
#include "module.h"
#include <math.h>

void reclass_as_bool(ast_node* node, bool value);
void reclass_as_num(ast_node* node, float result, bool is_integer);
void reclass_node(ast_node* node, ast_node* node1, ast_node* node2);

ast_node* evaluate_expression(ast_node* node, bool determinable);
ast_node* assign_variable(ast_node* node, bool determinable);
ast_node* simplify_expression(ast_node* node, bool determinable);
ast_node* initial_iteration(ast_node* node, hashtable* symbols);
ast_node* find_conditions(ast_node* node);

ast_node* replace_if_statements(ast_node* node, bool determinable);
ast_node* remove_variables(ast_node* node);
ast_node* find_expressions(ast_node* node, bool determinable);
void reset_variables(ast_node* node);

void optimize(module* mod, ast_node* node);