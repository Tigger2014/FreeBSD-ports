<?php
/*
 * status_wireguard.php
 *
 * part of pfSense (https://www.pfsense.org)
 * Copyright (c) 2021 R. Christian McDonald
 * Copyright (c) 2020 Ascrod
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

##|+PRIV
##|*IDENT=page-vpn-wg-status
##|*NAME=Status: WireGuard
##|*DESCR=Allow access to the 'Status: WireGuard' page.
##|*MATCH=status_wireguard.php*
##|-PRIV

$shortcut_section = "wireguard";
$pgtitle = array(gettext("Status"), "WireGuard");

require_once("guiconfig.inc");
require_once("functions.inc");
require_once("shortcuts.inc");
require_once("/usr/local/pkg/wireguard/wg.inc");

include("head.inc");

$tab_array = array();
$tab_array[] = array(gettext("Tunnels"), false, "vpn_wg.php");
//$tab_array[] = array(gettext("Wizards"), false, "#");
$tab_array[] = array(gettext("Status"), true, "status_wireguard.php");

add_package_tabs("wireguard", $tab_array);
display_top_tabs($tab_array);

?>

<div class="panel panel-default">
	<div class="panel-heading">
		<h2 class="panel-title">Connection Status</h2>
	</div>
	<div class="panel-body">
		<dl class="dl-horizontal">
			<pre><?php echo wg_status(); ?></pre>
		</dl>
    </div>
</div>

<div class="panel panel-default">
	<div class="panel-heading">
		<h2 class="panel-title">Interface Status</h2>
	</div>
	<div class="panel-body">
		<dl class="dl-horizontal">
			<pre><?php echo wg_interface_status(); ?></pre>
		</dl>
    </div>
</div>

<div class="panel panel-default">
	<div class="panel-heading">
		<h2 class="panel-title">WireGuard Version</h2>
	</div>
	<div class="panel-body">
		<dl class="dl-horizontal">
			<pre><?php echo wg_version(); ?></pre>
		</dl>
    </div>
</div>

<div class="panel panel-default">
	<div class="panel-heading">
		<h2 class="panel-title">Kernel Module Status</h2>
	</div>
	<div class="panel-body">
		<dl class="dl-horizontal">
			<pre><?php echo wg_kmod_status(); ?></pre>
		</dl>
    </div>
</div>

<?php include("foot.inc"); ?>
