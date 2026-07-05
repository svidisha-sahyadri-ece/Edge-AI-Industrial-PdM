/**
  ******************************************************************************
  * @file    ae_model_data_params.c
  * @author  AST Embedded Analytics Research Platform
  * @date    2026-04-14T00:39:09+0530
  * @brief   AI Tool Automatic Code Generator for Embedded NN computing
  ******************************************************************************
  * Copyright (c) 2026 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  ******************************************************************************
  */

#include "ae_model_data_params.h"


/**  Activations Section  ****************************************************/
ai_handle g_ae_model_activations_table[1 + 2] = {
  AI_HANDLE_PTR(AI_MAGIC_MARKER),
  AI_HANDLE_PTR(NULL),
  AI_HANDLE_PTR(AI_MAGIC_MARKER),
};




/**  Weights Section  ********************************************************/
AI_ALIGNED(32)
const ai_u64 s_ae_model_weights_array_u64[129] = {
  0x1ef3bf00f2ff343bU, 0x8b203d810bd20952U, 0x164f0b61068881e6U, 0xafb4b8308de9d4c4U,
  0x38c82c70fb3939a2U, 0xe4e6a9d921228581U, 0x33a81d7f022dfe18U, 0xce1134f323d83f52U,
  0xe3e827810e16c614U, 0xa2d31dc525f637e2U, 0x813e49c57f06ff0fU, 0xf33156afc0452c34U,
  0x4bf8fd18fa0af7e9U, 0x848107c832100216U, 0x3a181b0a2211173fU, 0xe312e87f28d8b8f4U,
  0x7f3bc101e8d1072eU, 0x31c95bfe8413d02U, 0xd9813bc22ca2fd9bU, 0xdb1af3ea0528a999U,
  0x2a7f1923f40328f8U, 0xbdd302e313e56453U, 0x20cb1bcdd3a781cfU, 0xe7126aea2f1f81e8U,
  0xb35a7f36cda010cfU, 0x59918711eeddbcddU, 0xf881cec5453e0375U, 0x3c63c9dc0d5648afU,
  0x2c70000004aU, 0x710000022fU, 0xffffff61000000dbU, 0x19e000000d2U,
  0xffffffac00000084U, 0x40000001e6U, 0x15900000047U, 0x5600000043U,
  0x7f72113ae96414d4U, 0xd52b403229dcea5aU, 0x404b03630047febU, 0xf4fae7b7163620d2U,
  0xf8ada264819ab74bU, 0x499792edf3ab01f9U, 0xd781a625f9cea99bU, 0x8fd6bc16e3a3ec2dU,
  0x4ec5edec037f6018U, 0x30ff5cd93e58dd26U, 0x592d1a7fc3530fecU, 0x23a7fe5bf8240609U,
  0xd90d7f14dd2cee11U, 0x47149a3035895d77U, 0xe13c133d62717f4dU, 0xf8ae1619d4add11dU,
  0xde00000119U, 0x2dffffffbaU, 0xba000000caU, 0x1b0000002cU,
  0x2aeabf7fc9a6c721U, 0x59314642e6fa7f5cU, 0x965fae0a6dfff87fU, 0xc69d7f4bec22e359U,
  0xffffff960000010bU, 0x7a000000efU, 0x7f04ee1eef819daeU, 0x74977fbba081fef0U,
  0x14c0157fbf52be7fU, 0x4281c66ea97f26dbU, 0x3500000000U, 0xffffff5400000000U,
  0x1300000052U, 0x15e0000004dU, 0xe3bc76847f96fba6U, 0x1003c27f9cacced4U,
  0x7f2ff5d8e7cb290bU, 0x84f71d179448128U, 0xf25afd287f312f5bU, 0xef67f72b302ef9bU,
  0x1b7fd4b852a070afU, 0x7fc3c1f5f82430e5U, 0x30ce77fdd110dc5U, 0x2be34e54ec97f2eU,
  0xeeb37f3d90c5026bU, 0x984c88b96581a2a0U, 0x817fb1badbd74fb8U, 0x5e17f01a7b96d1eU,
  0xf2ab04e081b6e96bU, 0x5581e2d7cda57721U, 0xabffffff21U, 0x960000006dU,
  0x167fffffe33U, 0xa5000001efU, 0x1560000003fU, 0xffffffa200000088U,
  0xfffffdc2fffffff2U, 0x1400000000U, 0xe16d3bf236bbb581U, 0x2db1524173b1d1c0U,
  0xffe1c5244a64a6b3U, 0xeafaf1057f091d57U, 0x772d57f1d32df90U, 0xaa891c2d115bbde8U,
  0x9fd9c5e5736b2c1U, 0x293f117ffd66c710U, 0xe43e43ce743a704U, 0x810af2040257d705U,
  0x131b907fc1a6301bU, 0x1b365a0dd4ea192fU, 0x81e2d5133bcdde1aU, 0xe13d0207e31f1c2dU,
  0xbc8146a00172c4f4U, 0x13b062794c4f0959U, 0xf8b053c7fae3756U, 0x3fcc49dd217aa2cbU,
  0x16ee5422ddb23225U, 0x5f03d4810ee3fd89U, 0x84d798740fddd04U, 0x81278ff2aae7d7c0U,
  0xe56b4bf4dacac9d8U, 0x8d687fad6defb04eU, 0xa0b241ee5222e24U, 0xbcf64601e981b63eU,
  0x1f76e56435e1fa81U, 0xc34760b0732aa6aaU, 0xfffffed0fffffeb3U, 0xfffffff1fffffd70U,
  0xfffffd5affffff25U, 0xfffffbb6ffffff85U, 0xe4fffffd6aU, 0xfffffe240000028bU,
  0xffffffb9ffffff98U,
};


ai_handle g_ae_model_weights_table[1 + 2] = {
  AI_HANDLE_PTR(AI_MAGIC_MARKER),
  AI_HANDLE_PTR(s_ae_model_weights_array_u64),
  AI_HANDLE_PTR(AI_MAGIC_MARKER),
};

