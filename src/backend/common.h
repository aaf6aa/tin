#pragma once

#include <stdarg.h>

// TODO: This file needs to be cleaned up

//
// Define string table
//
#define MAX_STRING_COUNT 1024
char *string_table[MAX_STRING_COUNT];
int string_table_index = 0;

//
// Define instruction table
//
#define MAX_INSTRUCTION_COUNT 4096
char *instruction_table[MAX_INSTRUCTION_COUNT];
int instruction_table_index = 0;

//
// Define data table
//
#define MAX_DATA_COUNT 4096
char *data_table[MAX_INSTRUCTION_COUNT];
int data_table_index = 0;

//
// Define variable hashtable
//
#define MAX_VARIABLE_COUNT 1024
hashtable* variable_table;

void add_to_instruction_table( const char* fmt, ... ) 
{
    va_list args;

    va_start( args, fmt );
    char* instruction = malloc( 1024 );
    vsprintf( instruction, fmt, args );
    // Add newline to the end
    strcat( instruction, "\n" );
    va_end( args );

    instruction_table[instruction_table_index++] = instruction;
}

#define add_instruction( fmt, ... ) add_to_instruction_table( "\t" fmt, ##__VA_ARGS__ )

/*
 * Prologue / epilogue
 * One main step to the calling convention is to add the prologue and epilogue.
 * This is where guarantees are made about the stack and registers.
 * These guarantees are:
 * - The stack pointer (sp) will have the same value when exiting the function
 *   as it did when entering the function.
 * - The s registers will have the same values when exiting the function as
 *   they did when entering the function.
 * - The function will return to the value stored in the return address
 *   register (ra).
 */

// TODO: These should eventually become mature functions
#define start_function( name ) add_to_instruction_table( "%s:", name ); \
                               add_instruction( "addi sp, sp, -16" ); \
                               add_instruction( "sw ra, 12(sp)" ); \
                               add_instruction( "sw s0, 8(sp)" ); \
                               add_instruction( "addi s0, sp, 16" );

#define end_function() add_instruction( "lw ra, 12(sp)" ); \
                       add_instruction( "lw s0, 8(sp)" ); \
                       add_instruction( "addi sp, sp, 16" ); \
                       add_instruction( "jr ra" )

#define add_string( str ) string_table[string_table_index++] = str
#define add_data( ... ) data_table[data_table_index++] = __VA_ARGS__
#define write_to_file( ... ) fprintf( file, __VA_ARGS__ )

#define compiler_error( ... ) fprintf( stderr, "Error: " __VA_ARGS__ ); \
                            exit( 1 )

#define call_function( name ) add_instruction( "call %s", name )

bool function_is_main( ast_node* node ) 
{
    return strcmp( node->value.symbol->name, "main" ) == 0;
}

char* get_function_name( ast_node* node ) 
{
    char* name = NULL;

    for ( int i = 0; i < node->children->size; i++ )
    {
        ast_node* child = (ast_node*)vector_get_item( node->children, i );
        switch( child->type )  
        {
            case AstSymbol:
                name = child->value.symbol->name;

                if ( function_is_main( child ) ) 
                {
                    name = "__start";
                }
                else 
                {
                    name = malloc( strlen( name ) + 4 );
                    sprintf( name, "fn__%s", child->value.symbol->name );
                }
                break;
            default:
                break;
        }
    }

    return name;
}

#ifdef TIN_DEBUG_VERBOSE
    #define add_comment( fmt, ... ) add_to_instruction_table( "# " fmt, ##__VA_ARGS__ )
    #define add_newline() instruction_table[instruction_table_index++] = "\n"
    #define trace( ... ) printf( __VA_ARGS__ ); printf( "\n" )
#else
    #define trace( ... )
    #define add_comment( ... )
    #define add_newline()
#endif // TIN_DEBUG_VERBOSE