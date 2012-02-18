<domain type = 'kvm'>
	<name>demo1</name>
	<title>a first demo xml to create a vm.</title>
	<memory>131072</memory>
	<vcpu>1</vcpu>
	<os>
		<type arch='i686' machine='pc'>hvm</type>
		<boot dev='hd'/>
		<bootmenu enable='no'/>
	</os>
	<feature>
		<acpi/>
		<apic/>
		<pae/>
	</feature>
	<clock offset='localtime'/>
	<on_poweroff>destroy</on_poweroff>
	<on_reboot>restart</on_reboot>
	<on_crash>restart</on_crash>
	<devices>
		<emulator>/usr/bin/qemu</emulator>
		<disk type='file' device='disk'>
			<source file='/home/ww/Work/libvirt/w1.disk.xm'/>
			<target dev='vda' bus='virtio'/>
			<driver name='qemu' type='qcow2'/>
		</disk>
		<interface type='bridge'>
			<source bridge='virbr0'/>
			<mac address='1c:75:08:59:f6:22'/>
		</interface>
		<input type='mouse' bus='ps2'/>
		<graphics type='vnc' port='5910' autoport='no' listen='127.0.0.1'/>
		<serial type='pty'>
			<target port='0'/>
			<source path='/dev/pts/1'/>
		</serial>
		<console type='pty'>
			<target port='0'/>
			<source path='/dev/pts/2'/>
		</console>
	</devices>
</domain>
