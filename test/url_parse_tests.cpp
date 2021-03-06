/*
    Simple parser for URL:s as specified by RFC1738 ( http://www.ietf.org/rfc/rfc1738.txt )

    version 1.0, June, 2014

	Copyright (C) 2014- Fredrik Kihlander

	This software is provided 'as-is', without any express or implied
	warranty.  In no event will the authors be held liable for any damages
	arising from the use of this software.

	Permission is granted to anyone to use this software for any purpose,
	including commercial applications, and to alter it and redistribute it
	freely, subject to the following restrictions:

	1. The origin of this software must not be misrepresented; you must not
	   claim that you wrote the original software. If you use this software
	   in a product, an acknowledgment in the product documentation would be
	   appreciated but is not required.
	2. Altered source versions must be plainly marked as such, and must not be
	   misrepresented as being the original software.
	3. This notice may not be removed or altered from any source distribution.

	Fredrik Kihlander
*/

#include "greatest.h"
#include <http_client/url.h>

TEST full_url_parse()
{
	parsed_url* parsed = parse_url( "http://user:pass@testurl.com:8080/sub/resource.file", 0x0, 0 );
	if( parsed == 0x0 )
		FAILm( "failed to parse url" );

	ASSERT_STR_EQ(               "http", parsed->scheme );
	ASSERT_STR_EQ(        "testurl.com", parsed->host );
	ASSERT_STR_EQ( "/sub/resource.file", parsed->path );
	ASSERT_STR_EQ(               "user", parsed->user );
	ASSERT_STR_EQ(               "pass", parsed->pass );

	ASSERT_EQ( 8080, parsed->port );

	free( parsed );

	return 0;
}

TEST url_no_scheme_with_port()
{
	parsed_url* parsed = parse_url( "testurl.com:8080", 0x0, 0 );
	if( parsed == 0x0 )
		FAILm( "failed to parse url" );

	ASSERT_STR_EQ( "testurl.com", parsed->host );
	ASSERT_STR_EQ( "/", parsed->path );

	ASSERT_EQ( 8080, parsed->port );
	ASSERT_EQ( 0x0,  parsed->scheme );
	ASSERT_EQ( 0x0,  parsed->user );
	ASSERT_EQ( 0x0,  parsed->pass );

	free( parsed );

	return 0;
}

TEST url_no_host()
{
	parsed_url* parsed = parse_url( "file:///sub/resource.file", 0x0, 0 );
	if( parsed == 0x0 )
		FAILm( "failed to parse url" );

	ASSERT_STR_EQ(          "localhost", parsed->host );
	ASSERT_STR_EQ(               "file", parsed->scheme );
	ASSERT_STR_EQ( "/sub/resource.file", parsed->path );

	ASSERT_EQ( 0x0, parsed->user );
	ASSERT_EQ( 0x0, parsed->pass );
	ASSERT_EQ(   0, parsed->port );

	free( parsed );

	return 0;
}

TEST default_port_parse()
{
	parsed_url* parsed;

	parsed = parse_url( "http://testurl.com", 0x0, 0 );
	if( parsed == 0x0 )
		FAILm( "failed to parse url" );

	ASSERT_STR_EQ(        "http", parsed->scheme );
	ASSERT_STR_EQ( "testurl.com", parsed->host );
	ASSERT_STR_EQ(           "/", parsed->path );
	ASSERT_EQ(  80, parsed->port );
	ASSERT_EQ( 0x0, parsed->user );
	ASSERT_EQ( 0x0, parsed->pass );

	free( parsed );

	parsed = parse_url( "ftp://testurl.com", 0x0, 0 );
	if( parsed == 0x0 )
		FAILm( "failed to parse url" );

	ASSERT_STR_EQ(         "ftp", parsed->scheme );
	ASSERT_STR_EQ( "testurl.com", parsed->host );
	ASSERT_STR_EQ(           "/", parsed->path );
	ASSERT_EQ(  21, parsed->port );
	ASSERT_EQ( 0x0, parsed->user );
	ASSERT_EQ( 0x0, parsed->pass );

	free( parsed );

	return 0;
}

TEST default_scheme_parse()
{
	parsed_url* parsed;

	parsed = parse_url( "testurl.com", 0x0, 0 );
	if( parsed == 0x0 )
		FAILm( "failed to parse url" );

	ASSERT_STR_EQ( "testurl.com", parsed->host );
	ASSERT_STR_EQ(           "/", parsed->path );

	ASSERT_EQ( 0x0, parsed->scheme );
	ASSERT_EQ(   0, parsed->port );
	ASSERT_EQ( 0x0, parsed->user );
	ASSERT_EQ( 0x0, parsed->pass );

	free( parsed );

	return 0;
}

TEST default_scheme_with_user_parse()
{
	parsed_url* parsed;

	parsed = parse_url( "hej:hopp@testurl.com", 0x0, 0 );
	if( parsed == 0x0 )
		FAILm( "failed to parse url" );

	ASSERT_EQ( 0x0, parsed->scheme );
	ASSERT_EQ(   0, parsed->port );
	ASSERT_STR_EQ( "testurl.com", parsed->host );
	ASSERT_STR_EQ(           "/", parsed->path );
	ASSERT_STR_EQ(         "hej", parsed->user );
	ASSERT_STR_EQ(        "hopp", parsed->pass );

	free( parsed );

	return 0;
}


GREATEST_SUITE( url_parse )
{
	RUN_TEST( full_url_parse );
	RUN_TEST( url_no_host );
	RUN_TEST( url_no_scheme_with_port );
	RUN_TEST( default_port_parse );
	RUN_TEST( default_scheme_parse );
	RUN_TEST( default_scheme_with_user_parse );
}

GREATEST_MAIN_DEFS();

int main( int argc, char **argv )
{
    GREATEST_MAIN_BEGIN();
    RUN_SUITE( url_parse );
    GREATEST_MAIN_END();
}

