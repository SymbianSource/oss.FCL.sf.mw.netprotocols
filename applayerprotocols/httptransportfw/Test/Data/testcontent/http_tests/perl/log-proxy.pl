#!/usr/bin/perl
# Copyright (c) 2003-2009 Nokia Corporation and/or its subsidiary(-ies).
# All rights reserved.
# This component and the accompanying materials are made available
# under the terms of "Eclipse Public License v1.0"
# which accompanies this distribution, and is available
# at the URL "http://www.eclipse.org/legal/epl-v10.html".
#
# Initial Contributors:
# Nokia Corporation - initial contribution.
#
# Contributors:
#
# Description:
# log-proxy <remote host> <port> [<localport> [<color>]]
# simple logging proxy - echo's all data to stdout.  Give the host to connect
# and the port on that host.  The local server will default to that port, or
# you may specify a new port for the local host proxy. Give '-color' to 'color'
# to get colorized output (ok, I'm lazy and not parsing options nicely - but I
# may in a later version).
# written 11/02/1999 by Evan K. Langlois (ekl@wans.net)
# 
#

use IO::Socket;
use IO::Select;
use POSIX;

$SIG{'INT'} = sub { COLOR($white,"\nkilled\n"); exit; };

$buflen = 24; 	# 24 bytes at a time - don't change this!
$DEBUG = 0;	# only 0 or 1 right now

# color numbers
($grey,$red,$green,$yellow,$blue,$magenta,$cyan,$white,$no,$bold) = (30..39);

# parse command line
($host,$port,$localport,$color) = @ARGV;
$localport = $port unless $localport;

# open server port
$serv = new IO::Socket::INET (	LocalHost => `hostname`,
				LocalPort => $localport,
				Proto	  => 'tcp',
				Listen	  => 5,
				Reuse	  => 1
			);
die "Socket could not be created.  Reason: $!\n" unless $serv;
$DATE = `date`;

# main loop
COLOR ($magenta,"-- Server Started on $DATE");
while ($client = $serv->accept()) {
	$HOST = $client->peerhost;
	$PORT = $client->peerport;
	COLOR ($blue,"-- Connect from $HOST:$PORT\n");
	# non-block IO with select
	fcntl ($client, F_SETFL(), O_NONBLOCK());
	$readable_handles = new IO::Select();
	$readable_handles->add($client);

	# see if we are an http proxy
	($new_readable) = IO::Select->select
					($readable_handles,undef,undef,1);
	if ($new_readable) {
		$bytes_read = sysread ($client, $_, ($buflen * 2));
		if (/^GET /) {
			($service,$host) = m#GET (\w+)://([\w|\.]+)[\s|/]#;
			$port = getservbyname ($service,'tcp');
		};
		HEX ($yellow,'->',substr ($_,0,$buflen));
		HEX ($yellow,'->',substr ($_,$buflen,$buflen));
		DEBUG ("Got $host : $port from $_\n");
	};
	$sock = new IO::Socket::INET (	PeerAddr => $host,
					PeerPort => $port,
					Proto	  => 'tcp',
				);
	die "Socket to $host:$port could not be created." . 
					"  Reason: $!\n" unless $sock;
	$DATE = `date`;
	COLOR ($blue,"-- Connected to $host:$port on $DATE");

	# non-blocking IO with select();
	fcntl ($sock, F_SETFL(), O_NONBLOCK());
	$readable_handles->add($sock);
	$socket_open = 1;

	# Pass on socket information
	print $sock $_ if ($new_readable);
	if (length > $buflen) {
		HEX ($cyan,'<-',substr($_,0,$buflen));
		HEX ($cyan,'<-',substr($_,$buflen));
	}
	DEBUG ("Sending $_ to server\n");

	# internal connection loop
	while ($socket_open) {
		DEBUG ("Selecting ...");
		($new_readable) = IO::Select->select
					($readable_handles,undef,undef,undef);
		foreach $read (@$new_readable) {
			DEBUG ("$read is ready to be read");
			if ($read == $client) {
				DEBUG ("client");
				$bytes_read = sysread ($client, $_, $buflen);
				if ($bytes_read == 0) {
					$readable_handles->remove($client);
					undef $socket_open;
				};
				print $sock $_;
				HEX ($yellow,'->',$_);
			} elsif ($read == $sock) {
				DEBUG ("sock");
				$bytes_read = sysread ($sock, $_, $buflen);
				if ($bytes_read == 0) {
					$readable_handles->remove($sock);
					undef $socket_open;
				};
				print $client $_;
				HEX ($cyan,'<-',$_);
			} else {
				last unless $socket_open;
				COLOR ($bold,"-- unknown socket $read: $!\n");
			};
		};
	};
	# connection lost
	close ($sock);
	close ($client);
	$DATE = `date`;
	COLOR ($blue,"-- Closed Connection on $DATE");
	COLOR ($white,""); 
};
close ($serv);
exit;

sub DEBUG {
	my ($arg) = @_;
	COLOR ($red,">*< DEBUG: $arg\n") if $DEBUG > 0;
}

sub COLOR {
	my ($outcolor,$outstring) = @_;
	if ($color) {
		print "\c[[01;${outcolor}m${outstring}";
	} else {
		print "$outstring";
	};
};

sub HEX {
	my ($outcolor,$ochar,$outdata) = @_;
	my $hex;
	my @lines;
	local $_;
	@lines = split (/([^\cM\n\0]+[\cM\n\0]+)/,$outdata);
	foreach $_ (@lines) {
		next if length == 0;
		$hex = unpack "H48",$_;
		$_ =~ tr/0-9A-Za-z~`!@#$%^&*()_+-={}[]:" ;'<>,.?\|\//./c;
	COLOR ($outcolor, (sprintf "%2s %-48s  %-24s\n",$ochar,$hex,$_));
	};
};

