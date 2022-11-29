/**
  ******************************************************************************
  * @file    ecg_data.c
  * @author  AST Embedded Analytics Research Platform
  * @date    Sun May  8 18:01:15 2022
  * @brief   AI Tool Automatic Code Generator for Embedded NN computing
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2022 STMicroelectronics.
  * All rights reserved.
  *
  * This software component is licensed by ST under Ultimate Liberty license
  * SLA0044, the "License"; You may not use this file except in compliance with
  * the License. You may obtain a copy of the License at:
  *                             www.st.com/SLA0044
  *
  ******************************************************************************
  */
#include "ecg_data.h"
#include "ai_platform_interface.h"


AI_ALIGNED(32)
const ai_u64 s_ecg_weights_array_u64[ 684 ] = {
  0x7f0000007fU, 0x8959e23581198114U, 0x1d813c92b03c7f81U, 0x220d8145b2817fceU,
  0x298700001a76U, 0xffffd57300003c48U, 0xffffd6ba00004508U, 0x1b790000336eU,
  0x7f7acae82fd851e5U, 0xedceda04c38159e6U, 0xca81555a13231413U, 0x2bad0a597f0676efU,
  0x252b379dfe7fd055U, 0x10c5ff405f46d7fU, 0x1bc78105b32c41f0U, 0xb9663dedca811e2bU,
  0x60efffff636U, 0xfffff6c3ffffffbdU, 0xfffffbfdfffffcaaU, 0xfffffba200000b28U,
  0xc9818194810981a8U, 0x7f1dcf81a87f347fU, 0xba6b0ac28a3c2c09U, 0x0U,
  0x0U, 0x0U, 0x0U, 0x2a7fd9df06b194f0U,
  0x253381ddd518271cU, 0xefe32c81fe00d8d6U, 0xb3fbe41a13d77fa8U, 0x2a81f21c2624a1feU,
  0x1dd8cb0502ffff7fU, 0xf5fa064b7ff4d130U, 0xb27f365c5751fc32U, 0xffffffbc0000004aU,
  0x474ffffff9bU, 0xfffffb76fffffe49U, 0x27200000a32U, 0xe1fe0812e1f3cbdcU,
  0x331dedffc18e8115U, 0xe3e747e616ea4d0eU, 0x1da1edc05cbf7f2U, 0xc4294a51de2e053fU,
  0xcb413611503813e9U, 0x1d0e424ff5ed1939U, 0xdb5be1fe27b3cfdfU, 0x67300000000U,
  0xfffffaa0000000fdU, 0xfffffcb9fffff1f4U, 0x6defffff56fU, 0x46cadf05dd0d132aU,
  0x17f2083ec4df27fdU, 0x3b0be1e1f41aea06U, 0xf920e40e021812e0U, 0x20d2e0d010d30408U,
  0xec1ace0f2e132ffU, 0xf9dac9be0eb081fcU, 0x5bd43a0021e8300fU, 0x96b3000098c3U,
  0xb013000090cdU, 0x8f3b0000a024U, 0xa6ad00009c14U, 0xea47090c81dbfa09U,
  0x35abf87ff70623eeU, 0x815244fbfdb526d5U, 0x6649d6b13581ee2eU, 0x7f3768ebc5ea832bU,
  0x9aea01c823b28781U, 0x2b083b9aa9ffc081U, 0xc97f9c67fffd9bafU, 0xfffffd1400000517U,
  0xfffff48100000a98U, 0x1bd2fffff93bU, 0x29200000ba8U, 0x1bd8f3181342573U,
  0x232fd05c2e18c77fU, 0x1b3aa87f29d76a15U, 0x8af8d6f4a0a27ff0U, 0x414c31b6dc7f4930U,
  0x8187b28b02bf3411U, 0xe0877f8cf6328689U, 0x42e212ebaf81310eU, 0xde2d85d8d17f2c37U,
  0x9cf655387f893127U, 0x7fbc52b5d2372a10U, 0x81f3ebcfa94e92b1U, 0x3d0ce65300fbc57fU,
  0x243d7ff7ac21ae92U, 0x4fae371eb8ad2981U, 0x3e3606d6c47fe776U, 0x3d0000005dU,
  0xd05ffffffe1U, 0x21dfffffe16U, 0x802fffffc30U, 0x5c8fffffce4U,
  0xfffffd84fffffe2aU, 0xfffffcd900000493U, 0x2a8fffffce5U, 0xdd22c9139cf17f7fU,
  0xbd815a5193817f7fU, 0xb68181057fae0a16U, 0x7fdcf5817f03a273U, 0x7fab23815c7fa7a7U,
  0xfeb07f2dbf249dacU, 0x0U, 0x0U, 0x0U,
  0x0U, 0x0U, 0x0U, 0x0U,
  0x0U, 0xc7da36f3d4b2735bU, 0xc77fec51ec6c1aebU, 0xddfa3a3babe12ac0U,
  0x81c55d22baffadd9U, 0x53b3566fec452d03U, 0x81f7bd62d993a826U, 0x13e709e140c145d5U,
  0xd1ce8125f31b2ab5U, 0xedc1f95b7fd053ddU, 0xe00c42035440e00eU, 0x8b37307f24f119cbU,
  0x52e4dcd6969d1acfU, 0xd821d68ebde04738U, 0xbbcfde90813e07f7U, 0x325b5c35151a12U,
  0x220df98118ff9404U, 0x46da7ffce55046e5U, 0x19dcfef21f2fe99bU, 0x2d81dadacc3fee4dU,
  0x37d1d13be9231032U, 0x3dd00000251U, 0x126000004f1U, 0xffffff4affffff7fU,
  0x7b000000b8U, 0x223000006d7U, 0xe01b0161535481e9U, 0x5a7fccb0dd4e4331U,
  0xaa7f16a6ffe75d10U, 0x84e5191566e919e3U, 0x8ba896fdf10d8143U, 0x522237f0ea28b13bU,
  0xb0facd3e03e50381U, 0x4a437d7f31d8527fU, 0xab363bcecae9f12fU, 0x5c1d9a0429f28192U,
  0xc7ec8101aff6dd59U, 0x5632d5e49caad997U, 0xfffffad37f960da8U, 0xfffff9a0fffffbd3U,
  0x44700001504U, 0xfffff05fffffff41U, 0xf8700000ddaU, 0x5e816218000006e4U,
  0x58fb134be5a33b0cU, 0x67cc4e81d22b29b8U, 0xb5cbe339e905fee4U, 0x815477a88459a281U,
  0x46cfc178cd2e2779U, 0xd9ed6a9e81bca9f6U, 0x7feed48a04f336f3U, 0x30365715e1f8120U,
  0x1faba0e0e9812c35U, 0x441ab9c604465ac8U, 0x5d8b324d6381aec2U, 0xe3ce24227fb422U,
  0xf5d97f4ae2d512a6U, 0xd781072eb333a12cU, 0x29de7f316b011e1bU, 0x81fdff3eeaec68f7U,
  0xe5cdc201b8f8c81cU, 0xe98e327f01181d48U, 0x7f7ef42c9e4d6afcU, 0xafab5a28625bf70fU,
  0x97235ec100f581d3U, 0xde7f1de5d0d31fecU, 0x24d124a2e1813f56U, 0xf6dbba20f7081507U,
  0x7e4604baae81c6feU, 0xb6123e0715521f81U, 0x32a7fd3a31c9248U, 0xd8817cce2f4fff2cU,
  0xe5dc407f2997952eU, 0xff5eb85d201b45daU, 0x20d6f0fe4dc17fb1U, 0xff7f5afabbfb04f6U,
  0x7f0a380132d4d1ffU, 0xd0ceec85581acae2U, 0xf102ab01fd7fbd2fU, 0x22bd1866247f41c8U,
  0xe001a22bc3767f36U, 0xd17f1da11cf2b4dfU, 0x766ab1e97fbc093bU, 0x54a0e99d18U,
  0x6bfffff58eU, 0xfffffbe20000110fU, 0xfffff928ffffed91U, 0xfffffd0500000017U,
  0xfffffe16fffffae2U, 0xfffffe82fffff845U, 0xfffff9d6fffffc24U, 0xffffffe1ffffffb5U,
  0xfffff90d0000036aU, 0x3a7000003a3U, 0xffffffd8fffffeedU, 0xfffffca6fffff3f7U,
  0xffffffe4000002faU, 0xfffffb8efffffe87U, 0xfffffe50ffffffccU, 0xc9812d7ffffffa61U,
  0xc0a48322a3327fc1U, 0xcb0939237fe0877fU, 0x81762e7ffaca137fU, 0x7f457f0d7f15ab0aU,
  0x811a7f5c023dbe1cU, 0x4c8e4685f3c5c9fdU, 0xf3a2b2c67fd9810aU, 0x881168a6527f7f81U,
  0x1c818f817f81f97fU, 0x7f7f81812f81813cU, 0xfa7f7ff222810581U, 0x1c453531U,
  0x0U, 0x0U, 0x0U, 0x0U,
  0x0U, 0x0U, 0x0U, 0x0U,
  0x0U, 0x0U, 0x0U, 0x0U,
  0x0U, 0x0U, 0x0U, 0xe919da2c00000000U,
  0xf7db251eefc10201U, 0xecd260eae284506U, 0x1e2212c72df11401U, 0xdf05bffac3811422U,
  0xd22ea0828b52e08U, 0xfcf0f96e2c0e12b9U, 0x2fdb21b908dc5c11U, 0xeabec1f3efdf0d7fU,
  0xc0bae229e5e8f2e5U, 0xdf812005e857bf06U, 0x79dcf7f01c14d88fU, 0x37102bf149e5fefdU,
  0x570b2881dd0e09d9U, 0xe5023f4ed30e40d5U, 0x56ed31363cc2670eU, 0xf2311923b5327800U,
  0xffea31a91cc5ffe9U, 0xd820880d3553eb28U, 0x537f3332dfe923b4U, 0x6fe38581f509f001U,
  0x2ff2e618421640f6U, 0xd114493ba75a0dacU, 0xd0855cd0e342305U, 0x47fc094705a4c644U,
  0xa5143192ebd3ab4U, 0xd2db54eae90b2aa3U, 0xfee38140072ffd04U, 0xbb46f912a6f91efcU,
  0x16377298c9ffbb06U, 0x16fdc809bd7f0df6U, 0xe6211eac1668f6ceU, 0x2b8cc06482a0b43U,
  0x1bffd91e81dee765U, 0xf3372ac41ed6c6fbU, 0x663acaf0aad7f96eU, 0xe4d3fc81040644d7U,
  0x460142ccd6d416e2U, 0x8d13bf15c41134e1U, 0xd4036405f3091fe3U, 0x23027cc4d3dU,
  0x6bffffff83U, 0xfffffe6b00000213U, 0x526000003f8U, 0xecffffffdeU,
  0xfc3bd53cfffff7ebU, 0xd3037f6d04367457U, 0x465f7ff82ae8c648U, 0x38eae2bc457fafecU,
  0x58ffd0e218de5ca7U, 0x7f5ca6c3d830e081U, 0x370bfd22ad5aa34aU, 0x7f6eb5903f9a79f4U,
  0x9b416ec3e9ad818fU, 0x22062cb8157fe887U, 0xc27f06653fd14bebU, 0xd37fee4e6a714faaU,
  0xee0fd801d6c6bad5U, 0xfffffc01fffff4ceU, 0x33b00000722U, 0x91fffff81eU,
  0xfffffdb900000a5eU, 0xffffffd0fffff812U, 0xea3681c7421ee8b7U, 0x12cd816c08d4f0e3U,
  0x2346993a113427beU, 0xc211e9819258e6e1U, 0xe42d8160ff64dbccU, 0xf827f1effdb981f1U,
  0xff523f411170bd4U, 0x1f0430e17ffdd48cU, 0xcf7f014168b610U, 0xf106410228c3ee81U,
  0xd6e4d9f681e52f7aU, 0x1a81611ec91917fbU, 0x4e224f07379503e0U, 0x1c7fb4f402448f81U,
  0xf7f9fef53208bffbU, 0xd0dbe0d1c3b3d6d6U, 0xf3184e28d8d2ed81U, 0x4df37f997fa8aaacU,
  0xbbed6dbc7fe27510U, 0x381d7fc7e6fa2741U, 0x11abf8256cf97f6fU, 0x184e7ff1e9c312feU,
  0x65f0f4b17551dd4U, 0x285e1fd92a357f32U, 0x81f0e2e78bbdc81cU, 0x6a79db49a0687ff8U,
  0x472503ded3babdfaU, 0x32813f2a15bdf181U, 0x19320be5dc25c6f1U, 0xe3c0059cc2dfec81U,
  0x8881fbd34d5b33b0U, 0x28ff3cf8b5bd25f9U, 0x811ba11588819f59U, 0x7ffc31a0b527d302U,
  0x28431168d6a2f1b1U, 0x1a3181693b231708U, 0x1195e13633e75229U, 0x4f3e51c1d9d77f83U,
  0x4f276b7f4da7fd56U, 0x55c838f9cfb54781U, 0x3a6fe80297317facU, 0xcbab1ddb9bf46e7eU,
  0xfffef9c7d581e487U, 0x9e0816ea6f2dff5U, 0xcd3f44e1058151c8U, 0x7f39b2d30fb640f3U,
  0x810fe79990307226U, 0x7fabddc456e0fb4cU, 0xa7fd031de351bbe5U, 0x93ddd6c2367fa840U,
  0x66fedafe0d110481U, 0xd4375db68102e2f4U, 0xb94ab92e29201108U, 0x867cf1b8d242e97fU,
  0xfd0181cdb4d7631cU, 0xecdbf9817b753d99U, 0x81ccedb502ba35c0U, 0x273ddd8e06af3418U,
  0xd48a2e5e81db2999U, 0x537fdfd4dd211207U, 0xcf8f42954ff41e2U, 0x564aeb50028d811eU,
  0x5c238137e4f78131U, 0x28f1c91b4c62c1ebU, 0x877ab7f6009d501U, 0x381b4b8a787f4c96U,
  0xe1bf27fab1cb389aU, 0x4181563e814cb37cU, 0x10ab9ceeeb39425aU, 0x781f347d43309fdU,
  0x2ed1814220100fe8U, 0x7fb6e010816ff9e3U, 0x33aca1044fe6db7fU, 0xc206c27f3d3e07dfU,
  0xc1060def7fecf234U, 0x3ab89f7fabbccf30U, 0x2b0466ece011cec2U, 0x8104a1cf81fd01e2U,
  0x819bedeac441e799U, 0x1b011c0ec943de14U, 0xfffff9d5ffffffc8U, 0xfffffda9fffffffdU,
  0xfffff37dfffff8c8U, 0xfffffdd3fffffc42U, 0xfffffa02fffff977U, 0xfffff7d6000001acU,
  0xfffffacbfffffb9eU, 0xfffff9c3fffff6ffU, 0xfffffc17fffff5ceU, 0xfffff667fffff631U,
  0xfffff9dafffffdacU, 0xffffff84fffffff9U, 0xfffffc29fffff629U, 0xa900000022U,
  0xfffff6c40000028dU, 0x351fffff710U, 0xfffff4ea000004adU, 0xfffffe1afffffe1fU,
  0xfffffd790000041cU, 0xfffffe7700000694U, 0xfffffd1efffffc45U, 0xfffff8b4fffff99dU,
  0xffffebf1fffff670U, 0xfffffa72fffffefbU, 0x39a000002c7U, 0xfffffe22fffffc48U,
  0x289fffffd1aU, 0xfffff7ed00000189U, 0xfffffda4ffffeb35U, 0xfffffd90000003d8U,
  0xfffffb28000002f1U, 0x770ffffff70U, 0xd6c9e17fef8191e7U, 0x3bd97f7fa5208113U,
  0x706a996721547f21U, 0xc40b7fda434d2adaU, 0x1f8158817f2e9e81U, 0x7ffaa4b0418160c9U,
  0x9f51b97f7f81e2a2U, 0xf713ca4ede03daffU, 0xe5cf50aa3231feddU, 0x1c50112281d1bf81U,
  0x81297ffef4fc2354U, 0x7f81353f8b81eb34U, 0x1511a10a588362cbU, 0xd04b7f81e8c2eaf5U,
  0x29ce3e5f5ee3be12U, 0x2b3d81fb01818126U, 0x3a1e7f0b7fbfa9d7U, 0xca1bb6cecf25ece3U,
  0x2e37e77f219d03a6U, 0x5fac4817f0ad3d6U, 0x480728f7653f8575U, 0x95f243c47f2bd33eU,
  0x2e26b705946a134dU, 0x6440ed616520321U, 0x2c813e1e99141269U, 0x57aedf5a2a152cf4U,
  0xfd0e42b7f7ff39cU, 0x45e2e182bef6c12fU, 0x8104024e527f58ddU, 0x4a7fe84c4e24bd81U,
  0x7cbe81d92ec3a2eeU, 0x31041f527f023181U, 0x7f9d642bf6288181U, 0x81811b9ff981f2d7U,
  0x398176f1e50fe981U, 0xd54b7013b81f8181U, 0xa507813f55cc7f24U, 0x5e8232ac10681d2U,
  0x8181cae7b5d47f81U, 0x7f81097fe7f44ddbU, 0x0U, 0x0U,
  0x0U, 0x0U, 0x0U, 0x0U,
  0x0U, 0x0U, 0x0U, 0x0U,
  0x0U, 0x0U, 0x0U, 0x0U,
  0x0U, 0x0U, 0x0U, 0x0U,
  0x0U, 0x0U, 0x0U, 0x0U,
  0x0U, 0x0U, 0x0U, 0x0U,
  0x0U, 0x0U, 0x0U, 0x0U,
  0x0U, 0x0U, 0x3802d127ca42ce2dU, 0xffc9b9cb0a09221cU,
  0xb70adb0dc2fc1918U, 0x3a3748663332020fU, 0x500d04be4dca34e8U, 0xe3c3f43ac0447fcdU,
  0xf854e90da2c46025U, 0x310dd8b18cd4bf1U, 0x8aa6ea13dbcb5db0U, 0x1a22eb3f022bc9ecU,
  0xb75ed025d7a6ec1cU, 0x64d9e1fdf813e8f1U, 0xe6fefd2e2b530aefU, 0x32b58c6ccd3ec21eU,
  0xfc1d2901df30ea26U, 0x7088c9fafc3d81c8U, 0x1dd69028c30c6af7U, 0x7fc869ec32f1ba5fU,
  0xe1e8cc6b3fdeee5U, 0xce20efb934e7f7ddU, 0x41f31742dbddf367U, 0xd9830d1127ee23efU,
  0xd3092531245d069dU, 0x2d22041cf63d2f15U, 0x5749c2edecdfe738U, 0x4e2c020ef80984e9U,
  0x5734e91df30ad239U, 0x11c445501df37f44U, 0x334142156845d1c2U, 0xd40cd4c54de6cf41U,
  0xeb5c1c532c20f40eU, 0x90f129f8bbf90612U, 0x16cad0b221281004U, 0x350d0ffe085fef0bU,
  0x90013fc241eff6aU, 0xfde21118d3e5dbcdU, 0x57fdb220c0950e9U, 0xf0f4f0cfe1e4de38U,
  0xeb1b49400f39d052U, 0x27263b37ba1bf712U, 0xd2df08a4cb226c35U, 0x7f1bcf0d4a19c4e5U,
  0xff07f1ca291a1e06U, 0xfe1b8f6fecd44feU, 0xccf4cee10213f905U, 0x67effc2734151d2fU,
  0xfe1604daed2d3111U, 0xb105cbd3ff7010f0U, 0xf9df0119e34cff7fU, 0x2817eae13b430621U,
  0xeb9b206fd2bcee4U, 0x16f4ff25b742f626U, 0x1034dd4d44b0e80dU, 0xd41ddf02ccf3d828U,
  0xd7cb4f060e1a4418U, 0xe6e53ef623ae2ffeU, 0xbfe5faf54ef0ec13U, 0x3007d8fef1373ddbU,
  0xe83e1f3dd4eb811eU, 0x1d5004f854e0d724U, 0xfae9d5a2e1ab08c9U, 0x1d0728d4fd09efa1U,
  0x4c342b0508e0fbceU, 0x29ab52df0d06eeffU, 0x2a1d0b3afade0610U, 0xe33d1baada1cfcdfU,
  0xe7d51504df1881f7U, 0x3d2726392c06cd5dU, 0x303d0859ed5905f4U, 0xfd04cfb9bbccf313U,
  0x1dfb632e0b4cf7aU, 0x11ddec0745f03745U, 0x4b41bd129400f20fU, 0xfc2a32212b2cf6e3U,
  0xd2b3bdfc17cc0502U, 0xea7f0ef2e3142ddbU, 0x24abfa174c324333U, 0x50b5f500d51d09bbU,
  0x1a2bf4d5a849d23aU, 0xf53e3d27e6fd18f1U, 0x87fd6198604f805U, 0x1657072438c62775U,
  0xfff7ad9931b642d8U, 0xd52ef2101d4c653eU, 0x37edfbd602cf5871U, 0xb6bb1a252dc24609U,
  0x1ef8ef9c3b58c8c0U, 0xa709e14011a320e8U, 0xf301d63814d710eaU, 0xdde6decdcbd4efefU,
  0x2f2a12c9d850ef05U, 0x93fbdf2ef93ddd4cU, 0xc4d258df20092abdU, 0x96e380ffb137f3cU,
  0xdee8e4d65936f02cU, 0xb3c01dca3c393a40U, 0xfffff50300000204U, 0x4cafffff755U,
  0xa32fffffa35U, 0xfffffadafffff90aU, 0xfffffb57fffffb52U, 0xfffffbd8000000b9U,
  0x2717d8ca2ae92f22U, 0x2befe1e8d7d839a5U, 0xb6e107d8f23cdfd5U, 0xfd032f1ff2061344U,
  0xaf5edfa170bf7f3U, 0x4e7dffde7fff7fbU, 0xc4e332f229110958U, 0xf71036cf0f1f16e7U,
  0xc2b8c2f3ecf5c812U, 0x2723ef1bfd0a0d0dU, 0x4e0ff18d5e714d8U, 0x3214ee3d0de83319U,
  0xfde60d1ffc25371dU, 0x6fef381f012d112U, 0xbefd523d8e3f5c3U, 0xf9fdcad712242ce1U,
  0x16eae613b4e732feU, 0x93a55ed321ef5f8U, 0xffffffddfffffb2fU, 0xfffffb21000001f5U,
  0x7e4000000a2U, 0x99bffffffccU, 0xfffffe0b0000069bU, 0xb8200000101U,
  0x26fb1bccf9078f01U, 0x19e94c1eead841e6U, 0xc407b5d3dfe7159bU, 0x29f9db2a2e23fe0dU,
  0xedff7f5e0ce2d3e6U, 0x2240ebd1a624fb0aU, 0x4d9dfecf9daf631U, 0x15df21c2c849fbU,
  0x2321f9f2cb2b59d6U, 0xf4f039c108bd763eU, 0x16170f37ea5a0cf7U, 0xf2bc06e833d9fc42U,
  0xe912efe5f3ce3224U, 0xf3e5f002052a5fb2U, 0xd90644ea06234a03U, 0xfe09d611ef19d93aU,
  0xf0f632e5deb7fd0cU, 0xb9f0e03de7f64700U, 0x5c2900005cf7U, 0x51970000590eU,
  0x53ac000056afU, 0x5ac80000571aU, 0x5a3b000051abU, 0x5384000054a7U,
  0xf37fcf1b2d60d4a6U, 0xfcd1f9d65aaef5faU, 0xadb98154ed1ef7c5U, 0xee12d93e7f1caf0eU,
  0x81e502042ed613dbU, 0xeb0cf30dfd0d1ffeU, 0xd2924c21e9d87f44U, 0x5be1fbca0af2e9bfU,
  0xfc0bd91d814eca06U, 0x1513e22b2a1d81bcU, 0xbefb54022415f520U, 0x4258fca5819b3af9U,
  0xbe7f1fb82edfcabcU, 0x941be61ad5dac4fbU, 0x440b17b044811939U, 0x92ded7aded813fafU,
  0x517f1557a308fdf2U, 0x1213651c79cc49f4U, 0x266900000cecU, 0xfffff52c00000f4cU,
  0x2059ffffdd8dU, 0xffffe06800000b41U, 0xffffca7600003629U, 0xffffdbbc0000024eU,
  0xa41dbbcaed19b8e1U, 0xeb2f81477fd64db1U, 0xedfc33da30271c13U, 0xbc433381a2492a55U,
  0xee59077fe9b6ee3fU, 0x933b320a46ae7a8U, 0xdc5516b481724e2cU, 0x201d17e7fb16U,
  0xffffc01b00002018U, 0xffffccf0ffffcb8cU, 0x997f677f60698db6U, 0x292e657381a9a0acU,
  0x1b638199eb9bb17fU, 0x3d7f54216d819f79U, 0x93bef51d7fb1bc82U, 0x77263d5050bd81U,
  0xffffff890000be81U, 0x24ffffffccU, 0x4200000048U, 0x760000015aU,
  0x7affffffbeU, 0x638e6cbfffffff93U, 0x74976d728b81U, 0x130U,
};



AI_API_DECLARE_BEGIN

/*!
 * @brief Get network weights array pointer as a handle ptr.
 * @ingroup ecg_data
 * @return a ai_handle pointer to the weights array
 */
AI_DEPRECATED
AI_API_ENTRY
ai_handle ai_ecg_data_weights_get(void)
{
  static const ai_u8* const s_ecg_weights_map[1 + 2] = {
    AI_PTR(AI_MAGIC_MARKER),
    AI_PTR(s_ecg_weights_array_u64),
    AI_PTR(AI_MAGIC_MARKER)
  };

  return AI_HANDLE_PTR(s_ecg_weights_map);

}


/*!
 * @brief Get network params configuration data structure.
 * @ingroup ecg_data
 * @return true if a valid configuration is present, false otherwise
 */
AI_API_ENTRY
ai_bool ai_ecg_data_params_get(ai_handle network, ai_network_params* params)
{
  if (!(network && params)) return false;
  
  static ai_buffer s_ecg_data_map_activations[AI_ECG_DATA_ACTIVATIONS_COUNT] = {
    AI_BUFFER_OBJ_INIT(AI_BUFFER_FORMAT_U8, 1, 1, AI_ECG_DATA_ACTIVATIONS_SIZE, 1, NULL)
  };

  const ai_buffer_array map_activations = 
    AI_BUFFER_ARRAY_OBJ_INIT(AI_FLAG_NONE, AI_ECG_DATA_ACTIVATIONS_COUNT, s_ecg_data_map_activations);
  
  static ai_buffer s_ecg_data_map_weights[AI_ECG_DATA_WEIGHTS_COUNT] = {
    AI_BUFFER_OBJ_INIT(AI_BUFFER_FORMAT_U8, 1, 1, 5472, 1, s_ecg_weights_array_u64),
    
  };

  const ai_buffer_array map_weights = 
    AI_BUFFER_ARRAY_OBJ_INIT(AI_FLAG_NONE, AI_ECG_DATA_WEIGHTS_COUNT, s_ecg_data_map_weights);

  return ai_platform_bind_network_params(network, params, &map_weights, &map_activations);
}


AI_API_DECLARE_END
