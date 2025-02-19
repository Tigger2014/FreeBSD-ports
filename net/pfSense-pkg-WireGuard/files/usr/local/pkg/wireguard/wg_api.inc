<?php
/*
 * wg_api.inc
 *
 * part of pfSense (https://www.pfsense.org)
 * Copyright (c) 2021 Rubicon Communications, LLC (Netgate)
 * Copyright (c) 2021 R. Christian McDonald
 * All rights reserved.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

require_once('globals.inc');
require_once('config.inc');
require_once('pfsense-utils.inc');
require_once('system.inc');
require_once('pkg-utils.inc');
require_once('service-utils.inc');
require_once('util.inc');

// Various paths to WireGuard binaries and confs
global $wgbinpath, $wgquickpath, $wgconfpath;
$wgbinpath = '/usr/local/bin/wg';
$wgquickpath = '/usr/local/bin/wg-quick';
$wgconfpath = '/etc/wireguard';

// Various paths to relevant binaries
global $ifconfigpath, $kldstatpath;
$ifconfigpath = '/sbin/ifconfig';
$kldstatpath = '/sbin/kldstat';
$pkgpath = '/usr/sbin/pkg';

// Experimenting with different prefixes, need to consult with Netgate
global $wgifprefix;
$wgifprefix = 'wg';

// Get WireGuard status
function wg_status() {
	global $wgbinpath;

	$status = array();
	exec("{$wgbinpath} show", $status);

	$output = implode("\n", $status);
	return htmlspecialchars($output);
}

// Get WireGuard interface status
function wg_interface_status($ifgroup = "wg") {
	global $ifconfigpath;

	$status = array();
	exec("{$ifconfigpath} -a -g {$ifgroup}", $status);

	$output = implode("\n", $status);
	return htmlspecialchars($output);
}

// Get WireGuard kmod status
function wg_kmod_status($kmod = 'if_wg.ko') {
	global $kldstatpath;

	$output = exec("{$kldstatpath} | grep {$kmod}");
	
	return htmlspecialchars($output);
}

// Get wg and tools version
function wg_version($pkgs = 'wireguard') {
	global $pkgpath;

	$packages = array();
	exec("{$pkgpath} info | grep {$pkgs}", $packages);

	$output = implode("\n", $packages);
	return htmlspecialchars($output);
}

// Generate private key
function genKeyPair($json = false) {
	global $wgbinpath;

	$privkey = exec("{$wgbinpath} genkey");
	$pubkey = genPubKey($privkey);

	$res = array('privkey' => $privkey, 'pubkey' => $pubkey);
	return $json ? json_encode($res) : $res;
}

// Compose the public key from a provided private key
function genPubKey($privkey) {
	global $wgbinpath;

	$pubkey = exec("echo {$privkey} | {$wgbinpath} pubkey");

	return $pubkey;
}

// Generate a pre-shared key
function genPSK() {
	global $wgbinpath;

	$psk = exec("{$wgbinpath} genpsk");

	return $psk;
}

// Return the next available WireGuard port
function next_wg_port() {
	global $config;

	init_config_arr(array('installedpackages', 'wireguard', 'tunnel'));
	$tunnels = &$config['installedpackages']['wireguard']['tunnel'];

	for ($idx=51820; $idx<65535; $idx++) {
		// Check to see if the port is already in use
		$found = false;
		foreach ($tunnels as $tunnel) {
			if ($tunnel['interface']['listenport'] == $idx) {
				$found = true;
				break;
			}
		}

		// If not, it can be used
		if (!$found) {
			return $idx;
		}
	}

	return 51820;
}

// Return the next available WireGuard interface
function next_wg_if() {
	global $config, $wgifprefix;
	init_config_arr(array('installedpackages', 'wireguard', 'tunnel'));
	$tunnels = &$config['installedpackages']['wireguard']['tunnel'];

	$used_ifs = array();
	foreach ($tunnels as $tunnel) {
		$used_ifs[] = $tunnel['name'];
	}

	for ($ifnum = 0; $ifnum < 32768; $ifnum++) {
		$want_if = "{$wgifprefix}{$ifnum}";
		if (!in_array($want_if, $used_ifs)) {
			return $want_if;
		}
	}
	return -1;
}

function wg_default_mtu() { return 1420; }

?>