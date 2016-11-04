
#include "Crc32__Test.h"

#include <sstream>

#include "../../../CommonSrc/crc32.h"



Crc32__Test::Crc32__Test(){
	unitTest = new UnitTest("Crc32__Test");

	Crc32::init();

	{
		// TEST TABLE
		const uint32_t crc32Table[] = {
			0x0 ,// 0
			0x77073096 ,// 1
			0xee0e612c ,// 2
			0x990951ba ,// 3
			0x76dc419 ,// 4
			0x706af48f ,// 5
			0xe963a535 ,// 6
			0x9e6495a3 ,// 7
			0xedb8832 ,// 8
			0x79dcb8a4 ,// 9
			0xe0d5e91e ,// 10
			0x97d2d988 ,// 11
			0x9b64c2b ,// 12
			0x7eb17cbd ,// 13
			0xe7b82d07 ,// 14
			0x90bf1d91 ,// 15
			0x1db71064 ,// 16
			0x6ab020f2 ,// 17
			0xf3b97148 ,// 18
			0x84be41de ,// 19
			0x1adad47d ,// 20
			0x6ddde4eb ,// 21
			0xf4d4b551 ,// 22
			0x83d385c7 ,// 23
			0x136c9856 ,// 24
			0x646ba8c0 ,// 25
			0xfd62f97a ,// 26
			0x8a65c9ec ,// 27
			0x14015c4f ,// 28
			0x63066cd9 ,// 29
			0xfa0f3d63 ,// 30
			0x8d080df5 ,// 31
			0x3b6e20c8 ,// 32
			0x4c69105e ,// 33
			0xd56041e4 ,// 34
			0xa2677172 ,// 35
			0x3c03e4d1 ,// 36
			0x4b04d447 ,// 37
			0xd20d85fd ,// 38
			0xa50ab56b ,// 39
			0x35b5a8fa ,// 40
			0x42b2986c ,// 41
			0xdbbbc9d6 ,// 42
			0xacbcf940 ,// 43
			0x32d86ce3 ,// 44
			0x45df5c75 ,// 45
			0xdcd60dcf ,// 46
			0xabd13d59 ,// 47
			0x26d930ac ,// 48
			0x51de003a ,// 49
			0xc8d75180 ,// 50
			0xbfd06116 ,// 51
			0x21b4f4b5 ,// 52
			0x56b3c423 ,// 53
			0xcfba9599 ,// 54
			0xb8bda50f ,// 55
			0x2802b89e ,// 56
			0x5f058808 ,// 57
			0xc60cd9b2 ,// 58
			0xb10be924 ,// 59
			0x2f6f7c87 ,// 60
			0x58684c11 ,// 61
			0xc1611dab ,// 62
			0xb6662d3d ,// 63
			0x76dc4190 ,// 64
			0x1db7106 ,// 65
			0x98d220bc ,// 66
			0xefd5102a ,// 67
			0x71b18589 ,// 68
			0x6b6b51f ,// 69
			0x9fbfe4a5 ,// 70
			0xe8b8d433 ,// 71
			0x7807c9a2 ,// 72
			0xf00f934 ,// 73
			0x9609a88e ,// 74
			0xe10e9818 ,// 75
			0x7f6a0dbb ,// 76
			0x86d3d2d ,// 77
			0x91646c97 ,// 78
			0xe6635c01 ,// 79
			0x6b6b51f4 ,// 80
			0x1c6c6162 ,// 81
			0x856530d8 ,// 82
			0xf262004e ,// 83
			0x6c0695ed ,// 84
			0x1b01a57b ,// 85
			0x8208f4c1 ,// 86
			0xf50fc457 ,// 87
			0x65b0d9c6 ,// 88
			0x12b7e950 ,// 89
			0x8bbeb8ea ,// 90
			0xfcb9887c ,// 91
			0x62dd1ddf ,// 92
			0x15da2d49 ,// 93
			0x8cd37cf3 ,// 94
			0xfbd44c65 ,// 95
			0x4db26158 ,// 96
			0x3ab551ce ,// 97
			0xa3bc0074 ,// 98
			0xd4bb30e2 ,// 99
			0x4adfa541 ,// 100
			0x3dd895d7 ,// 101
			0xa4d1c46d ,// 102
			0xd3d6f4fb ,// 103
			0x4369e96a ,// 104
			0x346ed9fc ,// 105
			0xad678846 ,// 106
			0xda60b8d0 ,// 107
			0x44042d73 ,// 108
			0x33031de5 ,// 109
			0xaa0a4c5f ,// 110
			0xdd0d7cc9 ,// 111
			0x5005713c ,// 112
			0x270241aa ,// 113
			0xbe0b1010 ,// 114
			0xc90c2086 ,// 115
			0x5768b525 ,// 116
			0x206f85b3 ,// 117
			0xb966d409 ,// 118
			0xce61e49f ,// 119
			0x5edef90e ,// 120
			0x29d9c998 ,// 121
			0xb0d09822 ,// 122
			0xc7d7a8b4 ,// 123
			0x59b33d17 ,// 124
			0x2eb40d81 ,// 125
			0xb7bd5c3b ,// 126
			0xc0ba6cad ,// 127
			0xedb88320 ,// 128
			0x9abfb3b6 ,// 129
			0x3b6e20c ,// 130
			0x74b1d29a ,// 131
			0xead54739 ,// 132
			0x9dd277af ,// 133
			0x4db2615 ,// 134
			0x73dc1683 ,// 135
			0xe3630b12 ,// 136
			0x94643b84 ,// 137
			0xd6d6a3e ,// 138
			0x7a6a5aa8 ,// 139
			0xe40ecf0b ,// 140
			0x9309ff9d ,// 141
			0xa00ae27 ,// 142
			0x7d079eb1 ,// 143
			0xf00f9344 ,// 144
			0x8708a3d2 ,// 145
			0x1e01f268 ,// 146
			0x6906c2fe ,// 147
			0xf762575d ,// 148
			0x806567cb ,// 149
			0x196c3671 ,// 150
			0x6e6b06e7 ,// 151
			0xfed41b76 ,// 152
			0x89d32be0 ,// 153
			0x10da7a5a ,// 154
			0x67dd4acc ,// 155
			0xf9b9df6f ,// 156
			0x8ebeeff9 ,// 157
			0x17b7be43 ,// 158
			0x60b08ed5 ,// 159
			0xd6d6a3e8 ,// 160
			0xa1d1937e ,// 161
			0x38d8c2c4 ,// 162
			0x4fdff252 ,// 163
			0xd1bb67f1 ,// 164
			0xa6bc5767 ,// 165
			0x3fb506dd ,// 166
			0x48b2364b ,// 167
			0xd80d2bda ,// 168
			0xaf0a1b4c ,// 169
			0x36034af6 ,// 170
			0x41047a60 ,// 171
			0xdf60efc3 ,// 172
			0xa867df55 ,// 173
			0x316e8eef ,// 174
			0x4669be79 ,// 175
			0xcb61b38c ,// 176
			0xbc66831a ,// 177
			0x256fd2a0 ,// 178
			0x5268e236 ,// 179
			0xcc0c7795 ,// 180
			0xbb0b4703 ,// 181
			0x220216b9 ,// 182
			0x5505262f ,// 183
			0xc5ba3bbe ,// 184
			0xb2bd0b28 ,// 185
			0x2bb45a92 ,// 186
			0x5cb36a04 ,// 187
			0xc2d7ffa7 ,// 188
			0xb5d0cf31 ,// 189
			0x2cd99e8b ,// 190
			0x5bdeae1d ,// 191
			0x9b64c2b0 ,// 192
			0xec63f226 ,// 193
			0x756aa39c ,// 194
			0x26d930a ,// 195
			0x9c0906a9 ,// 196
			0xeb0e363f ,// 197
			0x72076785 ,// 198
			0x5005713 ,// 199
			0x95bf4a82 ,// 200
			0xe2b87a14 ,// 201
			0x7bb12bae ,// 202
			0xcb61b38 ,// 203
			0x92d28e9b ,// 204
			0xe5d5be0d ,// 205
			0x7cdcefb7 ,// 206
			0xbdbdf21 ,// 207
			0x86d3d2d4 ,// 208
			0xf1d4e242 ,// 209
			0x68ddb3f8 ,// 210
			0x1fda836e ,// 211
			0x81be16cd ,// 212
			0xf6b9265b ,// 213
			0x6fb077e1 ,// 214
			0x18b74777 ,// 215
			0x88085ae6 ,// 216
			0xff0f6a70 ,// 217
			0x66063bca ,// 218
			0x11010b5c ,// 219
			0x8f659eff ,// 220
			0xf862ae69 ,// 221
			0x616bffd3 ,// 222
			0x166ccf45 ,// 223
			0xa00ae278 ,// 224
			0xd70dd2ee ,// 225
			0x4e048354 ,// 226
			0x3903b3c2 ,// 227
			0xa7672661 ,// 228
			0xd06016f7 ,// 229
			0x4969474d ,// 230
			0x3e6e77db ,// 231
			0xaed16a4a ,// 232
			0xd9d65adc ,// 233
			0x40df0b66 ,// 234
			0x37d83bf0 ,// 235
			0xa9bcae53 ,// 236
			0xdebb9ec5 ,// 237
			0x47b2cf7f ,// 238
			0x30b5ffe9 ,// 239
			0xbdbdf21c ,// 240
			0xcabac28a ,// 241
			0x53b39330 ,// 242
			0x24b4a3a6 ,// 243
			0xbad03605 ,// 244
			0xcdd70693 ,// 245
			0x54de5729 ,// 246
			0x23d967bf ,// 247
			0xb3667a2e ,// 248
			0xc4614ab8 ,// 249
			0x5d681b02 ,// 250
			0x2a6f2b94 ,// 251
			0xb40bbe37 ,// 252
			0xc30c8ea1 ,// 253
			0x5a05df1b ,// 254
			0x2d02ef8d // 255		
		};
		const uint32_t testLen = sizeof(crc32Table)/sizeof(crc32Table[0]);
		for(uint32_t i=0;i<testLen;i++){
			std::stringstream ssErrorMsg;
			ssErrorMsg << "Crc32::getTableValue(" << i << ") failed";
			unitTest->assertEquals(ssErrorMsg.str(),(uint32_t)crc32Table[i],Crc32::getTableValue(i));
		}
	}


	{
		// TEST SEQUENCE #0
		const uint8_t testSequence0[] = {0x00};
		const uint32_t testLen = sizeof(testSequence0)/sizeof(testSequence0[0]);
		uint32_t crcValue = Crc32::getInitValue();
		for(uint32_t i=0;i<testLen;i++){
			Crc32::appendByte(testSequence0[i],crcValue);
		}
		unitTest->assertEquals("testSequence0 crc32 failed",(uint32_t)0xd202ef8d,crcValue);
	}

	{
		// TEST SEQUENCE #1
		const uint8_t testSequence1[] = {0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00};
		const uint32_t testLen = sizeof(testSequence1)/sizeof(testSequence1[0]);
		uint32_t crcValue = Crc32::getInitValue();
		for(uint32_t i=0;i<testLen;i++){
			Crc32::appendByte(testSequence1[i],crcValue);
		}
		unitTest->assertEquals("testSequence1 crc32 failed",(uint32_t)0x6522df69,crcValue);
	}

	{
		// TEST SEQUENCE #2
		const uint8_t testSequence2[] = {
			144 ,
			26 ,
			31 ,
			42 ,
			214 ,
			152 ,
			15 ,
			15 
		};
		const uint32_t testLen = sizeof(testSequence2)/sizeof(testSequence2[0]);
		uint32_t crcValue = Crc32::getInitValue();
		for(uint32_t i=0;i<testLen;i++){
			Crc32::appendByte(testSequence2[i],crcValue);
		}
		unitTest->assertEquals("testSequence2 crc32 failed",(uint32_t)0x17e521a0,crcValue);
	}

	{
		// TEST SEQUENCE #3
		const uint8_t testSequence3[] = {0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF};
		const uint32_t testLen = sizeof(testSequence3)/sizeof(testSequence3[0]);
		uint32_t crcValue = Crc32::getInitValue();
		for(uint32_t i=0;i<testLen;i++){
			Crc32::appendByte(testSequence3[i],crcValue);
		}
		unitTest->assertEquals("testSequence3 crc32 failed",(uint32_t)0x2144df1c,crcValue);
	}

	delete unitTest;
}

