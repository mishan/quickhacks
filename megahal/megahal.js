/* Copyright (C) 2013 Misha Nasledov <misha@nasledov.com>
 *
 * This program is free software; you can redistribute it and/or modify it
 * under the terms of the GNU General Public License as published by the Free
 * Software Foundation; either version 2 of the license or (at your option)
 * any later version.
 *
 * This program is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY
 * or FITNESS FOR A PARTICULAR PURPOSE.  See the Gnu Public License for more
 * details.
 *
 * You should have received a copy of the GNU General Public License along
 * with this program; if not, write to the Free Software Foundation, Inc.,
 * 675 Mass Ave, Cambridge, MA 02139, USA */

var ref = require('ref');
var ffi = require('ffi');
// XXX for test code
var sys = require('sys');

var libmegahal = ffi.Library('./libmegahal', {
    // These must be called before megahal_initialize()
    'megahal_setnobanner':    [ 'void',   [ ] ],
    // params: directory
    'megahal_setdirectory':   [ 'void',   [ 'string' ] ],

    // This must be called before using megahal.
    'megahal_initialize':     [ 'void',   [ ] ],

    // params: input, log (whether to write to megahal.log)
    // return: reply
    'megahal_do_reply':       [ 'string', ['string', 'int'] ],
    // params: input, log (whether to write to megahal.log)
    'megahal_learn_no_reply': [ 'void',   ['string', 'int'] ],

    // This must called when done to ensure everything is saved.
    'megahal_cleanup':        [ 'void',   [ ] ],
});

function MegaHAL(optParams) {
    if (optParams) {
        // TODO: other params?
        if (optParams.directory) {
            libmegahal.megahal_setdirectory(optParams.directory);
        }
    }
}

MegaHAL.prototype.init = function() {
    libmegahal.megahal_setnobanner();
    libmegahal.megahal_initialize();
}

MegaHAL.prototype.learn = function(input) {
    libmegahal.megahal_learn_no_reply(input, 1);
}

MegaHAL.prototype.reply = function(input) {
    return libmegahal.megahal_do_reply(input, 1);
}

MegaHAL.prototype.cleanup = function() {
    libmegahal.megahal_cleanup();
}



module.exports = MegaHAL;
// XXX: test code

var obj = new MegaHAL();
obj.init();
var stdin = process.openStdin();

stdin.addListener('data', function(d) {
    if (d == "\n") {
        obj.cleanup();
        process.exit(0);
    } else {
        console.log(obj.reply(d.toString()));
    }
});
