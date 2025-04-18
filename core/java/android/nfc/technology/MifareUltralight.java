/*
 * Copyright (C) 2010 The Android Open Source Project
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

package android.nfc.technology;

import java.io.IOException;

import android.nfc.NfcAdapter;
import android.nfc.Tag;
import android.os.Bundle;
import android.os.RemoteException;

/**
 * Concrete class for TagTechnology.MIFARE_ULTRALIGHT
 *
 * Mifare classic has n sectors, with varying sizes, although
 * they are at least the same pattern for any one mifare classic
 * product. Each sector has two keys. Authentication with the correct
 * key is needed before access to any sector.
 *
 * Each sector has k blocks.
 * Block size is constant across the whole mifare classic family.
 */
public final class MifareUltralight extends BasicTagTechnology {
    public static final int TYPE_ULTRALIGHT = 1;
    public static final int TYPE_ULTRALIGHT_C = 2;
    public static final int TYPE_UNKNOWN = 10;

    private static final int NXP_MANUFACTURER_ID = 0x04;

    private int mType;

    public MifareUltralight(NfcAdapter adapter, Tag tag, Bundle extras) throws RemoteException {
        super(adapter, tag, TagTechnology.MIFARE_ULTRALIGHT);

        // Check if this could actually be a Mifare
        NfcA a = (NfcA) tag.getTechnology(adapter, TagTechnology.NFC_A);

        mType = TYPE_UNKNOWN;

        if( a.getSak() == 0x00 && tag.getId()[0] == NXP_MANUFACTURER_ID ) {
            // could be UL or UL-C
            mType = TYPE_ULTRALIGHT;
        }
    }

    public int getType() {
        return mType;
    }

    // Methods that require connect()
    /**
     * @throws IOException
     */
    public byte[] readBlock(int block) throws IOException {
        byte[] blockread_cmd = { 0x30, (byte)block }; // phHal_eMifareRead
        return transceive(blockread_cmd);
    }

    /**
     * @throws IOException
     */
    /*
    public byte[] readOTP();
    public void writePage(int block, byte[] data);
    public void writeBlock(int block, byte[] data);
     */
}
