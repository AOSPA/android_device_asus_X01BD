/*
   Copyright (c) 2020 Paranoid Android

   Redistribution and use in source and binary forms, with or without
   modification, are permitted provided that the following conditions are
   met:
    * Redistributions of source code must retain the above copyright
      notice, this list of conditions and the following disclaimer.
    * Redistributions in binary form must reproduce the above
      copyright notice, this list of conditions and the following
      disclaimer in the documentation and/or other materials provided
      with the distribution.
    * Neither the name of The Linux Foundation nor the names of its
      contributors may be used to endorse or promote products derived
      from this software without specific prior written permission.

   THIS SOFTWARE IS PROVIDED "AS IS" AND ANY EXPRESS OR IMPLIED
   WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF
   MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NON-INFRINGEMENT
   ARE DISCLAIMED.  IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS
   BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
   CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
   SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR
   BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
   WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE
   OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN
   IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#include <stdlib.h>
#include <android-base/properties.h>
#define _REALLY_INCLUDE_SYS__SYSTEM_PROPERTIES_H_
#include <sys/_system_properties.h>
#include <stdio.h>
#include <sys/system_properties.h>

#include "vendor_init.h"
#include "property_service.h"

using android::base::GetProperty;
using android::init::property_set;

void dalvik_properties()
{
    struct sysinfo sys;

    sysinfo(&sys);
    if (sys.totalram > 4096ull * 1024 * 1024) {
        // Set for 6GB or more RAM
        property_set("dalvik.vm.heapstartsize", "16m");
        property_set("dalvik.vm.heapgrowthlimit", "256m");
        property_set("dalvik.vm.heapsize", "512m");
        property_set("dalvik.vm.heaptargetutilization", "0.5");
        property_set("dalvik.vm.heapmaxfree", "32m");
        property_set("dalvik.vm.heapminfree", "8m");
    } else if (sys.totalram > 3072ull * 1024 * 1024) {
        // Set for 4GB RAM
        property_set("dalvik.vm.heapstartsize", "8m");
        property_set("dalvik.vm.heapgrowthlimit", "192m");
        property_set("dalvik.vm.heapsize", "512m");
        property_set("dalvik.vm.heaptargetutilization", "0.6");
        property_set("dalvik.vm.heapmaxfree", "16m");
        property_set("dalvik.vm.heapminfree", "8m");
    } else {
        // Set for 2/3GB RAM
        property_set("dalvik.vm.heapstartsize", "8m");
        property_set("dalvik.vm.heapgrowthlimit", "192m");
        property_set("dalvik.vm.heapsize", "512m");
        property_set("dalvik.vm.heaptargetutilization", "0.75");
        property_set("dalvik.vm.heapmaxfree", "8m");
        property_set("dalvik.vm.heapminfree", "512k");
  }
}

void property_override(char const prop[], char const value[])
{
	prop_info *pi;

	pi = (prop_info*) __system_property_find(prop);
	if (pi)
		__system_property_update(pi, value, strlen(value));
	else
		__system_property_add(prop, strlen(prop), value, strlen(value));
}

void property_override_dual(char const system_prop[], char const vendor_prop[], const char value[])
{
	property_override(system_prop, value);
	property_override(vendor_prop, value);
}

void vendor_load_properties()
{
	property_override("ro.build.fingerprint", "google/walleye/walleye:8.1.0/OPM1.171019.011/4448085:user/release-keys");
	dalvik_properties();
}
