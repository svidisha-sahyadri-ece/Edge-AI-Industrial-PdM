/**
  ******************************************************************************
  * @file    ae_model.c
  * @author  AST Embedded Analytics Research Platform
  * @date    2026-04-14T00:39:09+0530
  * @brief   AI Tool Automatic Code Generator for Embedded NN computing
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2026 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  ******************************************************************************
  */


#include "ae_model.h"
#include "ae_model_data.h"

#include "ai_platform.h"
#include "ai_platform_interface.h"
#include "ai_math_helpers.h"

#include "core_common.h"
#include "core_convert.h"

#include "layers.h"



#undef AI_NET_OBJ_INSTANCE
#define AI_NET_OBJ_INSTANCE g_ae_model
 
#undef AI_AE_MODEL_MODEL_SIGNATURE
#define AI_AE_MODEL_MODEL_SIGNATURE     "0x51751291d9e0a859f6f7e3352a5f47f1"

#ifndef AI_TOOLS_REVISION_ID
#define AI_TOOLS_REVISION_ID     ""
#endif

#undef AI_TOOLS_DATE_TIME
#define AI_TOOLS_DATE_TIME   "2026-04-14T00:39:09+0530"

#undef AI_TOOLS_COMPILE_TIME
#define AI_TOOLS_COMPILE_TIME    __DATE__ " " __TIME__

#undef AI_AE_MODEL_N_BATCHES
#define AI_AE_MODEL_N_BATCHES         (1)

static ai_ptr g_ae_model_activations_map[1] = AI_C_ARRAY_INIT;
static ai_ptr g_ae_model_weights_map[1] = AI_C_ARRAY_INIT;



/**  Array declarations section  **********************************************/
/* Array#0 */
AI_ARRAY_OBJ_DECLARE(
  serving_default_keras_tensor0_output_array, AI_ARRAY_FORMAT_FLOAT|AI_FMT_FLAG_IS_IO,
  NULL, NULL, 14, AI_STATIC)

/* Array#1 */
AI_ARRAY_OBJ_DECLARE(
  conversion_0_output_array, AI_ARRAY_FORMAT_S8,
  NULL, NULL, 14, AI_STATIC)

/* Array#2 */
AI_ARRAY_OBJ_DECLARE(
  gemm_1_output_array, AI_ARRAY_FORMAT_S8,
  NULL, NULL, 16, AI_STATIC)

/* Array#3 */
AI_ARRAY_OBJ_DECLARE(
  gemm_2_output_array, AI_ARRAY_FORMAT_S8,
  NULL, NULL, 8, AI_STATIC)

/* Array#4 */
AI_ARRAY_OBJ_DECLARE(
  gemm_3_output_array, AI_ARRAY_FORMAT_S8,
  NULL, NULL, 4, AI_STATIC)

/* Array#5 */
AI_ARRAY_OBJ_DECLARE(
  gemm_4_output_array, AI_ARRAY_FORMAT_S8,
  NULL, NULL, 8, AI_STATIC)

/* Array#6 */
AI_ARRAY_OBJ_DECLARE(
  gemm_5_output_array, AI_ARRAY_FORMAT_S8,
  NULL, NULL, 16, AI_STATIC)

/* Array#7 */
AI_ARRAY_OBJ_DECLARE(
  gemm_6_output_array, AI_ARRAY_FORMAT_S8,
  NULL, NULL, 14, AI_STATIC)

/* Array#8 */
AI_ARRAY_OBJ_DECLARE(
  conversion_7_output_array, AI_ARRAY_FORMAT_FLOAT|AI_FMT_FLAG_IS_IO,
  NULL, NULL, 14, AI_STATIC)

/* Array#9 */
AI_ARRAY_OBJ_DECLARE(
  gemm_1_weights_array, AI_ARRAY_FORMAT_S8,
  NULL, NULL, 224, AI_STATIC)

/* Array#10 */
AI_ARRAY_OBJ_DECLARE(
  gemm_1_bias_array, AI_ARRAY_FORMAT_S32,
  NULL, NULL, 16, AI_STATIC)

/* Array#11 */
AI_ARRAY_OBJ_DECLARE(
  gemm_2_weights_array, AI_ARRAY_FORMAT_S8,
  NULL, NULL, 128, AI_STATIC)

/* Array#12 */
AI_ARRAY_OBJ_DECLARE(
  gemm_2_bias_array, AI_ARRAY_FORMAT_S32,
  NULL, NULL, 8, AI_STATIC)

/* Array#13 */
AI_ARRAY_OBJ_DECLARE(
  gemm_3_weights_array, AI_ARRAY_FORMAT_S8,
  NULL, NULL, 32, AI_STATIC)

/* Array#14 */
AI_ARRAY_OBJ_DECLARE(
  gemm_3_bias_array, AI_ARRAY_FORMAT_S32,
  NULL, NULL, 4, AI_STATIC)

/* Array#15 */
AI_ARRAY_OBJ_DECLARE(
  gemm_4_weights_array, AI_ARRAY_FORMAT_S8,
  NULL, NULL, 32, AI_STATIC)

/* Array#16 */
AI_ARRAY_OBJ_DECLARE(
  gemm_4_bias_array, AI_ARRAY_FORMAT_S32,
  NULL, NULL, 8, AI_STATIC)

/* Array#17 */
AI_ARRAY_OBJ_DECLARE(
  gemm_5_weights_array, AI_ARRAY_FORMAT_S8,
  NULL, NULL, 128, AI_STATIC)

/* Array#18 */
AI_ARRAY_OBJ_DECLARE(
  gemm_5_bias_array, AI_ARRAY_FORMAT_S32,
  NULL, NULL, 16, AI_STATIC)

/* Array#19 */
AI_ARRAY_OBJ_DECLARE(
  gemm_6_weights_array, AI_ARRAY_FORMAT_S8,
  NULL, NULL, 224, AI_STATIC)

/* Array#20 */
AI_ARRAY_OBJ_DECLARE(
  gemm_6_bias_array, AI_ARRAY_FORMAT_S32,
  NULL, NULL, 14, AI_STATIC)

/* Array#21 */
AI_ARRAY_OBJ_DECLARE(
  gemm_1_scratch0_array, AI_ARRAY_FORMAT_S16,
  NULL, NULL, 94, AI_STATIC)

/* Array#22 */
AI_ARRAY_OBJ_DECLARE(
  gemm_2_scratch0_array, AI_ARRAY_FORMAT_S16,
  NULL, NULL, 56, AI_STATIC)

/* Array#23 */
AI_ARRAY_OBJ_DECLARE(
  gemm_3_scratch0_array, AI_ARRAY_FORMAT_S16,
  NULL, NULL, 28, AI_STATIC)

/* Array#24 */
AI_ARRAY_OBJ_DECLARE(
  gemm_4_scratch0_array, AI_ARRAY_FORMAT_S16,
  NULL, NULL, 44, AI_STATIC)

/* Array#25 */
AI_ARRAY_OBJ_DECLARE(
  gemm_5_scratch0_array, AI_ARRAY_FORMAT_S16,
  NULL, NULL, 88, AI_STATIC)

/* Array#26 */
AI_ARRAY_OBJ_DECLARE(
  gemm_6_scratch0_array, AI_ARRAY_FORMAT_S16,
  NULL, NULL, 86, AI_STATIC)

/**  Array metadata declarations section  *************************************/
/* Int quant #0 */
AI_INTQ_INFO_LIST_OBJ_DECLARE(conversion_0_output_array_intq, AI_STATIC_CONST,
  AI_BUFFER_META_FLAG_SCALE_FLOAT|AI_BUFFER_META_FLAG_ZEROPOINT_S8, 1,
  AI_PACK_INTQ_INFO(
    AI_PACK_INTQ_SCALE(0.1953658163547516f),
    AI_PACK_INTQ_ZP(-32)))

/* Int quant #1 */
AI_INTQ_INFO_LIST_OBJ_DECLARE(gemm_1_output_array_intq, AI_STATIC_CONST,
  AI_BUFFER_META_FLAG_SCALE_FLOAT|AI_BUFFER_META_FLAG_ZEROPOINT_S8, 1,
  AI_PACK_INTQ_INFO(
    AI_PACK_INTQ_SCALE(0.158881276845932f),
    AI_PACK_INTQ_ZP(-128)))

/* Int quant #2 */
AI_INTQ_INFO_LIST_OBJ_DECLARE(gemm_1_weights_array_intq, AI_STATIC_CONST,
  AI_BUFFER_META_FLAG_SCALE_FLOAT|AI_BUFFER_META_FLAG_ZEROPOINT_S8, 16,
  AI_PACK_INTQ_INFO(
    AI_PACK_INTQ_SCALE(0.005143465008586645f, 0.004566964693367481f, 0.005064328201115131f, 0.005109802819788456f, 0.004999888129532337f, 0.005033360328525305f, 0.003959184046834707f, 0.005887086037546396f, 0.007302535697817802f, 0.00397837208583951f, 0.00414164736866951f, 0.007405872456729412f, 0.004696345422416925f, 0.006271218881011009f, 0.003430216806009412f, 0.006013200152665377f),
    AI_PACK_INTQ_ZP(0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0)))

/* Int quant #3 */
AI_INTQ_INFO_LIST_OBJ_DECLARE(gemm_2_output_array_intq, AI_STATIC_CONST,
  AI_BUFFER_META_FLAG_SCALE_FLOAT|AI_BUFFER_META_FLAG_ZEROPOINT_S8, 1,
  AI_PACK_INTQ_INFO(
    AI_PACK_INTQ_SCALE(0.23594354093074799f),
    AI_PACK_INTQ_ZP(-128)))

/* Int quant #4 */
AI_INTQ_INFO_LIST_OBJ_DECLARE(gemm_2_weights_array_intq, AI_STATIC_CONST,
  AI_BUFFER_META_FLAG_SCALE_FLOAT|AI_BUFFER_META_FLAG_ZEROPOINT_S8, 8,
  AI_PACK_INTQ_INFO(
    AI_PACK_INTQ_SCALE(0.00611592223867774f, 0.006520169787108898f, 0.004191880114376545f, 0.003652427112683654f, 0.006263727322220802f, 0.0053072660230100155f, 0.004879666492342949f, 0.005690878257155418f),
    AI_PACK_INTQ_ZP(0, 0, 0, 0, 0, 0, 0, 0)))

/* Int quant #5 */
AI_INTQ_INFO_LIST_OBJ_DECLARE(gemm_3_output_array_intq, AI_STATIC_CONST,
  AI_BUFFER_META_FLAG_SCALE_FLOAT|AI_BUFFER_META_FLAG_ZEROPOINT_S8, 1,
  AI_PACK_INTQ_INFO(
    AI_PACK_INTQ_SCALE(0.3197180926799774f),
    AI_PACK_INTQ_ZP(-128)))

/* Int quant #6 */
AI_INTQ_INFO_LIST_OBJ_DECLARE(gemm_3_weights_array_intq, AI_STATIC_CONST,
  AI_BUFFER_META_FLAG_SCALE_FLOAT|AI_BUFFER_META_FLAG_ZEROPOINT_S8, 4,
  AI_PACK_INTQ_INFO(
    AI_PACK_INTQ_SCALE(0.007292539346963167f, 0.006955404300242662f, 0.005686604883521795f, 0.005172796081751585f),
    AI_PACK_INTQ_ZP(0, 0, 0, 0)))

/* Int quant #7 */
AI_INTQ_INFO_LIST_OBJ_DECLARE(gemm_4_output_array_intq, AI_STATIC_CONST,
  AI_BUFFER_META_FLAG_SCALE_FLOAT|AI_BUFFER_META_FLAG_ZEROPOINT_S8, 1,
  AI_PACK_INTQ_INFO(
    AI_PACK_INTQ_SCALE(0.19417180120944977f),
    AI_PACK_INTQ_ZP(-128)))

/* Int quant #8 */
AI_INTQ_INFO_LIST_OBJ_DECLARE(gemm_4_weights_array_intq, AI_STATIC_CONST,
  AI_BUFFER_META_FLAG_SCALE_FLOAT|AI_BUFFER_META_FLAG_ZEROPOINT_S8, 8,
  AI_PACK_INTQ_INFO(
    AI_PACK_INTQ_SCALE(0.005548209883272648f, 0.007359462324529886f, 0.00477294297888875f, 0.0030500544235110283f, 0.006902509834617376f, 0.006109105888754129f, 0.0058279018849134445f, 0.0045106918551027775f),
    AI_PACK_INTQ_ZP(0, 0, 0, 0, 0, 0, 0, 0)))

/* Int quant #9 */
AI_INTQ_INFO_LIST_OBJ_DECLARE(gemm_5_output_array_intq, AI_STATIC_CONST,
  AI_BUFFER_META_FLAG_SCALE_FLOAT|AI_BUFFER_META_FLAG_ZEROPOINT_S8, 1,
  AI_PACK_INTQ_INFO(
    AI_PACK_INTQ_SCALE(0.11786554753780365f),
    AI_PACK_INTQ_ZP(-128)))

/* Int quant #10 */
AI_INTQ_INFO_LIST_OBJ_DECLARE(gemm_5_weights_array_intq, AI_STATIC_CONST,
  AI_BUFFER_META_FLAG_SCALE_FLOAT|AI_BUFFER_META_FLAG_ZEROPOINT_S8, 16,
  AI_PACK_INTQ_INFO(
    AI_PACK_INTQ_SCALE(0.003148880088701844f, 0.004525911062955856f, 0.006335472222417593f, 0.004385077860206366f, 0.002990316366776824f, 0.004915475845336914f, 0.0037950407713651657f, 0.005444379057735205f, 0.005311216227710247f, 0.0037112608551979065f, 0.003752009244635701f, 0.003928253892809153f, 0.004009694792330265f, 0.004290400538593531f, 0.0038102311082184315f, 0.003360255854204297f),
    AI_PACK_INTQ_ZP(0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0)))

/* Int quant #11 */
AI_INTQ_INFO_LIST_OBJ_DECLARE(gemm_6_output_array_intq, AI_STATIC_CONST,
  AI_BUFFER_META_FLAG_SCALE_FLOAT|AI_BUFFER_META_FLAG_ZEROPOINT_S8, 1,
  AI_PACK_INTQ_INFO(
    AI_PACK_INTQ_SCALE(0.18687166273593903f),
    AI_PACK_INTQ_ZP(-26)))

/* Int quant #12 */
AI_INTQ_INFO_LIST_OBJ_DECLARE(gemm_6_weights_array_intq, AI_STATIC_CONST,
  AI_BUFFER_META_FLAG_SCALE_FLOAT|AI_BUFFER_META_FLAG_ZEROPOINT_S8, 14,
  AI_PACK_INTQ_INFO(
    AI_PACK_INTQ_SCALE(0.0036748077254742384f, 0.004691524431109428f, 0.0030511864461004734f, 0.0034723985008895397f, 0.006900962442159653f, 0.005646260920912027f, 0.007025782484561205f, 0.004362692125141621f, 0.004642134066671133f, 0.004521738737821579f, 0.00406655902042985f, 0.003909378778189421f, 0.007714849431067705f, 0.003869027132168412f),
    AI_PACK_INTQ_ZP(0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0)))

/**  Tensor declarations section  *********************************************/
/* Tensor #0 */
AI_TENSOR_OBJ_DECLARE(
  conversion_0_output, AI_STATIC,
  0, 0x1,
  AI_SHAPE_INIT(4, 1, 14, 1, 1), AI_STRIDE_INIT(4, 1, 1, 14, 14),
  1, &conversion_0_output_array, &conversion_0_output_array_intq)

/* Tensor #1 */
AI_TENSOR_OBJ_DECLARE(
  conversion_7_output, AI_STATIC,
  1, 0x0,
  AI_SHAPE_INIT(4, 1, 14, 1, 1), AI_STRIDE_INIT(4, 4, 4, 56, 56),
  1, &conversion_7_output_array, NULL)

/* Tensor #2 */
AI_TENSOR_OBJ_DECLARE(
  gemm_1_bias, AI_STATIC,
  2, 0x0,
  AI_SHAPE_INIT(4, 1, 16, 1, 1), AI_STRIDE_INIT(4, 4, 4, 64, 64),
  1, &gemm_1_bias_array, NULL)

/* Tensor #3 */
AI_TENSOR_OBJ_DECLARE(
  gemm_1_output, AI_STATIC,
  3, 0x1,
  AI_SHAPE_INIT(4, 1, 16, 1, 1), AI_STRIDE_INIT(4, 1, 1, 16, 16),
  1, &gemm_1_output_array, &gemm_1_output_array_intq)

/* Tensor #4 */
AI_TENSOR_OBJ_DECLARE(
  gemm_1_scratch0, AI_STATIC,
  4, 0x0,
  AI_SHAPE_INIT(4, 1, 94, 1, 1), AI_STRIDE_INIT(4, 2, 2, 188, 188),
  1, &gemm_1_scratch0_array, NULL)

/* Tensor #5 */
AI_TENSOR_OBJ_DECLARE(
  gemm_1_weights, AI_STATIC,
  5, 0x1,
  AI_SHAPE_INIT(4, 14, 16, 1, 1), AI_STRIDE_INIT(4, 1, 14, 224, 224),
  1, &gemm_1_weights_array, &gemm_1_weights_array_intq)

/* Tensor #6 */
AI_TENSOR_OBJ_DECLARE(
  gemm_2_bias, AI_STATIC,
  6, 0x0,
  AI_SHAPE_INIT(4, 1, 8, 1, 1), AI_STRIDE_INIT(4, 4, 4, 32, 32),
  1, &gemm_2_bias_array, NULL)

/* Tensor #7 */
AI_TENSOR_OBJ_DECLARE(
  gemm_2_output, AI_STATIC,
  7, 0x1,
  AI_SHAPE_INIT(4, 1, 8, 1, 1), AI_STRIDE_INIT(4, 1, 1, 8, 8),
  1, &gemm_2_output_array, &gemm_2_output_array_intq)

/* Tensor #8 */
AI_TENSOR_OBJ_DECLARE(
  gemm_2_scratch0, AI_STATIC,
  8, 0x0,
  AI_SHAPE_INIT(4, 1, 56, 1, 1), AI_STRIDE_INIT(4, 2, 2, 112, 112),
  1, &gemm_2_scratch0_array, NULL)

/* Tensor #9 */
AI_TENSOR_OBJ_DECLARE(
  gemm_2_weights, AI_STATIC,
  9, 0x1,
  AI_SHAPE_INIT(4, 16, 8, 1, 1), AI_STRIDE_INIT(4, 1, 16, 128, 128),
  1, &gemm_2_weights_array, &gemm_2_weights_array_intq)

/* Tensor #10 */
AI_TENSOR_OBJ_DECLARE(
  gemm_3_bias, AI_STATIC,
  10, 0x0,
  AI_SHAPE_INIT(4, 1, 4, 1, 1), AI_STRIDE_INIT(4, 4, 4, 16, 16),
  1, &gemm_3_bias_array, NULL)

/* Tensor #11 */
AI_TENSOR_OBJ_DECLARE(
  gemm_3_output, AI_STATIC,
  11, 0x1,
  AI_SHAPE_INIT(4, 1, 4, 1, 1), AI_STRIDE_INIT(4, 1, 1, 4, 4),
  1, &gemm_3_output_array, &gemm_3_output_array_intq)

/* Tensor #12 */
AI_TENSOR_OBJ_DECLARE(
  gemm_3_scratch0, AI_STATIC,
  12, 0x0,
  AI_SHAPE_INIT(4, 1, 28, 1, 1), AI_STRIDE_INIT(4, 2, 2, 56, 56),
  1, &gemm_3_scratch0_array, NULL)

/* Tensor #13 */
AI_TENSOR_OBJ_DECLARE(
  gemm_3_weights, AI_STATIC,
  13, 0x1,
  AI_SHAPE_INIT(4, 8, 4, 1, 1), AI_STRIDE_INIT(4, 1, 8, 32, 32),
  1, &gemm_3_weights_array, &gemm_3_weights_array_intq)

/* Tensor #14 */
AI_TENSOR_OBJ_DECLARE(
  gemm_4_bias, AI_STATIC,
  14, 0x0,
  AI_SHAPE_INIT(4, 1, 8, 1, 1), AI_STRIDE_INIT(4, 4, 4, 32, 32),
  1, &gemm_4_bias_array, NULL)

/* Tensor #15 */
AI_TENSOR_OBJ_DECLARE(
  gemm_4_output, AI_STATIC,
  15, 0x1,
  AI_SHAPE_INIT(4, 1, 8, 1, 1), AI_STRIDE_INIT(4, 1, 1, 8, 8),
  1, &gemm_4_output_array, &gemm_4_output_array_intq)

/* Tensor #16 */
AI_TENSOR_OBJ_DECLARE(
  gemm_4_scratch0, AI_STATIC,
  16, 0x0,
  AI_SHAPE_INIT(4, 1, 44, 1, 1), AI_STRIDE_INIT(4, 2, 2, 88, 88),
  1, &gemm_4_scratch0_array, NULL)

/* Tensor #17 */
AI_TENSOR_OBJ_DECLARE(
  gemm_4_weights, AI_STATIC,
  17, 0x1,
  AI_SHAPE_INIT(4, 4, 8, 1, 1), AI_STRIDE_INIT(4, 1, 4, 32, 32),
  1, &gemm_4_weights_array, &gemm_4_weights_array_intq)

/* Tensor #18 */
AI_TENSOR_OBJ_DECLARE(
  gemm_5_bias, AI_STATIC,
  18, 0x0,
  AI_SHAPE_INIT(4, 1, 16, 1, 1), AI_STRIDE_INIT(4, 4, 4, 64, 64),
  1, &gemm_5_bias_array, NULL)

/* Tensor #19 */
AI_TENSOR_OBJ_DECLARE(
  gemm_5_output, AI_STATIC,
  19, 0x1,
  AI_SHAPE_INIT(4, 1, 16, 1, 1), AI_STRIDE_INIT(4, 1, 1, 16, 16),
  1, &gemm_5_output_array, &gemm_5_output_array_intq)

/* Tensor #20 */
AI_TENSOR_OBJ_DECLARE(
  gemm_5_scratch0, AI_STATIC,
  20, 0x0,
  AI_SHAPE_INIT(4, 1, 88, 1, 1), AI_STRIDE_INIT(4, 2, 2, 176, 176),
  1, &gemm_5_scratch0_array, NULL)

/* Tensor #21 */
AI_TENSOR_OBJ_DECLARE(
  gemm_5_weights, AI_STATIC,
  21, 0x1,
  AI_SHAPE_INIT(4, 8, 16, 1, 1), AI_STRIDE_INIT(4, 1, 8, 128, 128),
  1, &gemm_5_weights_array, &gemm_5_weights_array_intq)

/* Tensor #22 */
AI_TENSOR_OBJ_DECLARE(
  gemm_6_bias, AI_STATIC,
  22, 0x0,
  AI_SHAPE_INIT(4, 1, 14, 1, 1), AI_STRIDE_INIT(4, 4, 4, 56, 56),
  1, &gemm_6_bias_array, NULL)

/* Tensor #23 */
AI_TENSOR_OBJ_DECLARE(
  gemm_6_output, AI_STATIC,
  23, 0x1,
  AI_SHAPE_INIT(4, 1, 14, 1, 1), AI_STRIDE_INIT(4, 1, 1, 14, 14),
  1, &gemm_6_output_array, &gemm_6_output_array_intq)

/* Tensor #24 */
AI_TENSOR_OBJ_DECLARE(
  gemm_6_scratch0, AI_STATIC,
  24, 0x0,
  AI_SHAPE_INIT(4, 1, 86, 1, 1), AI_STRIDE_INIT(4, 2, 2, 172, 172),
  1, &gemm_6_scratch0_array, NULL)

/* Tensor #25 */
AI_TENSOR_OBJ_DECLARE(
  gemm_6_weights, AI_STATIC,
  25, 0x1,
  AI_SHAPE_INIT(4, 16, 14, 1, 1), AI_STRIDE_INIT(4, 1, 16, 224, 224),
  1, &gemm_6_weights_array, &gemm_6_weights_array_intq)

/* Tensor #26 */
AI_TENSOR_OBJ_DECLARE(
  serving_default_keras_tensor0_output, AI_STATIC,
  26, 0x0,
  AI_SHAPE_INIT(4, 1, 14, 1, 1), AI_STRIDE_INIT(4, 4, 4, 56, 56),
  1, &serving_default_keras_tensor0_output_array, NULL)



/**  Layer declarations section  **********************************************/


AI_TENSOR_CHAIN_OBJ_DECLARE(
  conversion_7_chain, AI_STATIC_CONST, 4,
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &gemm_6_output),
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &conversion_7_output),
  AI_TENSOR_LIST_OBJ_EMPTY,
  AI_TENSOR_LIST_OBJ_EMPTY
)

AI_LAYER_OBJ_DECLARE(
  conversion_7_layer, 7,
  NL_TYPE, 0x0, NULL,
  nl, node_convert,
  &conversion_7_chain,
  NULL, &conversion_7_layer, AI_STATIC, 
)

AI_TENSOR_CHAIN_OBJ_DECLARE(
  gemm_6_chain, AI_STATIC_CONST, 4,
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &gemm_5_output),
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &gemm_6_output),
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 2, &gemm_6_weights, &gemm_6_bias),
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &gemm_6_scratch0)
)

AI_LAYER_OBJ_DECLARE(
  gemm_6_layer, 6,
  DENSE_TYPE, 0x0, NULL,
  dense, forward_dense_integer_SSSA_ch,
  &gemm_6_chain,
  NULL, &conversion_7_layer, AI_STATIC, 
)

AI_TENSOR_CHAIN_OBJ_DECLARE(
  gemm_5_chain, AI_STATIC_CONST, 4,
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &gemm_4_output),
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &gemm_5_output),
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 2, &gemm_5_weights, &gemm_5_bias),
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &gemm_5_scratch0)
)

AI_LAYER_OBJ_DECLARE(
  gemm_5_layer, 5,
  DENSE_TYPE, 0x0, NULL,
  dense, forward_dense_integer_SSSA_ch,
  &gemm_5_chain,
  NULL, &gemm_6_layer, AI_STATIC, 
)

AI_TENSOR_CHAIN_OBJ_DECLARE(
  gemm_4_chain, AI_STATIC_CONST, 4,
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &gemm_3_output),
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &gemm_4_output),
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 2, &gemm_4_weights, &gemm_4_bias),
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &gemm_4_scratch0)
)

AI_LAYER_OBJ_DECLARE(
  gemm_4_layer, 4,
  DENSE_TYPE, 0x0, NULL,
  dense, forward_dense_integer_SSSA_ch,
  &gemm_4_chain,
  NULL, &gemm_5_layer, AI_STATIC, 
)

AI_TENSOR_CHAIN_OBJ_DECLARE(
  gemm_3_chain, AI_STATIC_CONST, 4,
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &gemm_2_output),
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &gemm_3_output),
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 2, &gemm_3_weights, &gemm_3_bias),
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &gemm_3_scratch0)
)

AI_LAYER_OBJ_DECLARE(
  gemm_3_layer, 3,
  DENSE_TYPE, 0x0, NULL,
  dense, forward_dense_integer_SSSA_ch,
  &gemm_3_chain,
  NULL, &gemm_4_layer, AI_STATIC, 
)

AI_TENSOR_CHAIN_OBJ_DECLARE(
  gemm_2_chain, AI_STATIC_CONST, 4,
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &gemm_1_output),
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &gemm_2_output),
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 2, &gemm_2_weights, &gemm_2_bias),
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &gemm_2_scratch0)
)

AI_LAYER_OBJ_DECLARE(
  gemm_2_layer, 2,
  DENSE_TYPE, 0x0, NULL,
  dense, forward_dense_integer_SSSA_ch,
  &gemm_2_chain,
  NULL, &gemm_3_layer, AI_STATIC, 
)

AI_TENSOR_CHAIN_OBJ_DECLARE(
  gemm_1_chain, AI_STATIC_CONST, 4,
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &conversion_0_output),
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &gemm_1_output),
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 2, &gemm_1_weights, &gemm_1_bias),
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &gemm_1_scratch0)
)

AI_LAYER_OBJ_DECLARE(
  gemm_1_layer, 1,
  DENSE_TYPE, 0x0, NULL,
  dense, forward_dense_integer_SSSA_ch,
  &gemm_1_chain,
  NULL, &gemm_2_layer, AI_STATIC, 
)

AI_TENSOR_CHAIN_OBJ_DECLARE(
  conversion_0_chain, AI_STATIC_CONST, 4,
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &serving_default_keras_tensor0_output),
  AI_TENSOR_LIST_OBJ_INIT(AI_FLAG_NONE, 1, &conversion_0_output),
  AI_TENSOR_LIST_OBJ_EMPTY,
  AI_TENSOR_LIST_OBJ_EMPTY
)

AI_LAYER_OBJ_DECLARE(
  conversion_0_layer, 0,
  NL_TYPE, 0x0, NULL,
  nl, node_convert,
  &conversion_0_chain,
  NULL, &gemm_1_layer, AI_STATIC, 
)


#if (AI_TOOLS_API_VERSION < AI_TOOLS_API_VERSION_1_5)

AI_NETWORK_OBJ_DECLARE(
  AI_NET_OBJ_INSTANCE, AI_STATIC,
  AI_BUFFER_INIT(AI_FLAG_NONE,  AI_BUFFER_FORMAT_U8,
    AI_BUFFER_SHAPE_INIT(AI_SHAPE_BCWH, 4, 1, 1032, 1, 1),
    1032, NULL, NULL),
  AI_BUFFER_INIT(AI_FLAG_NONE,  AI_BUFFER_FORMAT_U8,
    AI_BUFFER_SHAPE_INIT(AI_SHAPE_BCWH, 4, 1, 220, 1, 1),
    220, NULL, NULL),
  AI_TENSOR_LIST_IO_OBJ_INIT(AI_FLAG_NONE, AI_AE_MODEL_IN_NUM, &serving_default_keras_tensor0_output),
  AI_TENSOR_LIST_IO_OBJ_INIT(AI_FLAG_NONE, AI_AE_MODEL_OUT_NUM, &conversion_7_output),
  &conversion_0_layer, 0x1b9c4e5c, NULL)

#else

AI_NETWORK_OBJ_DECLARE(
  AI_NET_OBJ_INSTANCE, AI_STATIC,
  AI_BUFFER_ARRAY_OBJ_INIT_STATIC(
  	AI_FLAG_NONE, 1,
    AI_BUFFER_INIT(AI_FLAG_NONE,  AI_BUFFER_FORMAT_U8,
      AI_BUFFER_SHAPE_INIT(AI_SHAPE_BCWH, 4, 1, 1032, 1, 1),
      1032, NULL, NULL)
  ),
  AI_BUFFER_ARRAY_OBJ_INIT_STATIC(
  	AI_FLAG_NONE, 1,
    AI_BUFFER_INIT(AI_FLAG_NONE,  AI_BUFFER_FORMAT_U8,
      AI_BUFFER_SHAPE_INIT(AI_SHAPE_BCWH, 4, 1, 220, 1, 1),
      220, NULL, NULL)
  ),
  AI_TENSOR_LIST_IO_OBJ_INIT(AI_FLAG_NONE, AI_AE_MODEL_IN_NUM, &serving_default_keras_tensor0_output),
  AI_TENSOR_LIST_IO_OBJ_INIT(AI_FLAG_NONE, AI_AE_MODEL_OUT_NUM, &conversion_7_output),
  &conversion_0_layer, 0x1b9c4e5c, NULL)

#endif	/*(AI_TOOLS_API_VERSION < AI_TOOLS_API_VERSION_1_5)*/



/******************************************************************************/
AI_DECLARE_STATIC
ai_bool ae_model_configure_activations(
  ai_network* net_ctx, const ai_network_params* params)
{
  AI_ASSERT(net_ctx)

  if (ai_platform_get_activations_map(g_ae_model_activations_map, 1, params)) {
    /* Updating activations (byte) offsets */
    
    serving_default_keras_tensor0_output_array.data = AI_PTR(g_ae_model_activations_map[0] + 148);
    serving_default_keras_tensor0_output_array.data_start = AI_PTR(g_ae_model_activations_map[0] + 148);
    conversion_0_output_array.data = AI_PTR(g_ae_model_activations_map[0] + 204);
    conversion_0_output_array.data_start = AI_PTR(g_ae_model_activations_map[0] + 204);
    gemm_1_scratch0_array.data = AI_PTR(g_ae_model_activations_map[0] + 16);
    gemm_1_scratch0_array.data_start = AI_PTR(g_ae_model_activations_map[0] + 16);
    gemm_1_output_array.data = AI_PTR(g_ae_model_activations_map[0] + 0);
    gemm_1_output_array.data_start = AI_PTR(g_ae_model_activations_map[0] + 0);
    gemm_2_scratch0_array.data = AI_PTR(g_ae_model_activations_map[0] + 16);
    gemm_2_scratch0_array.data_start = AI_PTR(g_ae_model_activations_map[0] + 16);
    gemm_2_output_array.data = AI_PTR(g_ae_model_activations_map[0] + 128);
    gemm_2_output_array.data_start = AI_PTR(g_ae_model_activations_map[0] + 128);
    gemm_3_scratch0_array.data = AI_PTR(g_ae_model_activations_map[0] + 0);
    gemm_3_scratch0_array.data_start = AI_PTR(g_ae_model_activations_map[0] + 0);
    gemm_3_output_array.data = AI_PTR(g_ae_model_activations_map[0] + 56);
    gemm_3_output_array.data_start = AI_PTR(g_ae_model_activations_map[0] + 56);
    gemm_4_scratch0_array.data = AI_PTR(g_ae_model_activations_map[0] + 60);
    gemm_4_scratch0_array.data_start = AI_PTR(g_ae_model_activations_map[0] + 60);
    gemm_4_output_array.data = AI_PTR(g_ae_model_activations_map[0] + 0);
    gemm_4_output_array.data_start = AI_PTR(g_ae_model_activations_map[0] + 0);
    gemm_5_scratch0_array.data = AI_PTR(g_ae_model_activations_map[0] + 8);
    gemm_5_scratch0_array.data_start = AI_PTR(g_ae_model_activations_map[0] + 8);
    gemm_5_output_array.data = AI_PTR(g_ae_model_activations_map[0] + 184);
    gemm_5_output_array.data_start = AI_PTR(g_ae_model_activations_map[0] + 184);
    gemm_6_scratch0_array.data = AI_PTR(g_ae_model_activations_map[0] + 0);
    gemm_6_scratch0_array.data_start = AI_PTR(g_ae_model_activations_map[0] + 0);
    gemm_6_output_array.data = AI_PTR(g_ae_model_activations_map[0] + 200);
    gemm_6_output_array.data_start = AI_PTR(g_ae_model_activations_map[0] + 200);
    conversion_7_output_array.data = AI_PTR(g_ae_model_activations_map[0] + 0);
    conversion_7_output_array.data_start = AI_PTR(g_ae_model_activations_map[0] + 0);
    return true;
  }
  AI_ERROR_TRAP(net_ctx, INIT_FAILED, NETWORK_ACTIVATIONS);
  return false;
}




/******************************************************************************/
AI_DECLARE_STATIC
ai_bool ae_model_configure_weights(
  ai_network* net_ctx, const ai_network_params* params)
{
  AI_ASSERT(net_ctx)

  if (ai_platform_get_weights_map(g_ae_model_weights_map, 1, params)) {
    /* Updating weights (byte) offsets */
    
    gemm_1_weights_array.format |= AI_FMT_FLAG_CONST;
    gemm_1_weights_array.data = AI_PTR(g_ae_model_weights_map[0] + 0);
    gemm_1_weights_array.data_start = AI_PTR(g_ae_model_weights_map[0] + 0);
    gemm_1_bias_array.format |= AI_FMT_FLAG_CONST;
    gemm_1_bias_array.data = AI_PTR(g_ae_model_weights_map[0] + 224);
    gemm_1_bias_array.data_start = AI_PTR(g_ae_model_weights_map[0] + 224);
    gemm_2_weights_array.format |= AI_FMT_FLAG_CONST;
    gemm_2_weights_array.data = AI_PTR(g_ae_model_weights_map[0] + 288);
    gemm_2_weights_array.data_start = AI_PTR(g_ae_model_weights_map[0] + 288);
    gemm_2_bias_array.format |= AI_FMT_FLAG_CONST;
    gemm_2_bias_array.data = AI_PTR(g_ae_model_weights_map[0] + 416);
    gemm_2_bias_array.data_start = AI_PTR(g_ae_model_weights_map[0] + 416);
    gemm_3_weights_array.format |= AI_FMT_FLAG_CONST;
    gemm_3_weights_array.data = AI_PTR(g_ae_model_weights_map[0] + 448);
    gemm_3_weights_array.data_start = AI_PTR(g_ae_model_weights_map[0] + 448);
    gemm_3_bias_array.format |= AI_FMT_FLAG_CONST;
    gemm_3_bias_array.data = AI_PTR(g_ae_model_weights_map[0] + 480);
    gemm_3_bias_array.data_start = AI_PTR(g_ae_model_weights_map[0] + 480);
    gemm_4_weights_array.format |= AI_FMT_FLAG_CONST;
    gemm_4_weights_array.data = AI_PTR(g_ae_model_weights_map[0] + 496);
    gemm_4_weights_array.data_start = AI_PTR(g_ae_model_weights_map[0] + 496);
    gemm_4_bias_array.format |= AI_FMT_FLAG_CONST;
    gemm_4_bias_array.data = AI_PTR(g_ae_model_weights_map[0] + 528);
    gemm_4_bias_array.data_start = AI_PTR(g_ae_model_weights_map[0] + 528);
    gemm_5_weights_array.format |= AI_FMT_FLAG_CONST;
    gemm_5_weights_array.data = AI_PTR(g_ae_model_weights_map[0] + 560);
    gemm_5_weights_array.data_start = AI_PTR(g_ae_model_weights_map[0] + 560);
    gemm_5_bias_array.format |= AI_FMT_FLAG_CONST;
    gemm_5_bias_array.data = AI_PTR(g_ae_model_weights_map[0] + 688);
    gemm_5_bias_array.data_start = AI_PTR(g_ae_model_weights_map[0] + 688);
    gemm_6_weights_array.format |= AI_FMT_FLAG_CONST;
    gemm_6_weights_array.data = AI_PTR(g_ae_model_weights_map[0] + 752);
    gemm_6_weights_array.data_start = AI_PTR(g_ae_model_weights_map[0] + 752);
    gemm_6_bias_array.format |= AI_FMT_FLAG_CONST;
    gemm_6_bias_array.data = AI_PTR(g_ae_model_weights_map[0] + 976);
    gemm_6_bias_array.data_start = AI_PTR(g_ae_model_weights_map[0] + 976);
    return true;
  }
  AI_ERROR_TRAP(net_ctx, INIT_FAILED, NETWORK_WEIGHTS);
  return false;
}


/**  PUBLIC APIs SECTION  *****************************************************/



AI_DEPRECATED
AI_API_ENTRY
ai_bool ai_ae_model_get_info(
  ai_handle network, ai_network_report* report)
{
  ai_network* net_ctx = AI_NETWORK_ACQUIRE_CTX(network);

  if (report && net_ctx)
  {
    ai_network_report r = {
      .model_name        = AI_AE_MODEL_MODEL_NAME,
      .model_signature   = AI_AE_MODEL_MODEL_SIGNATURE,
      .model_datetime    = AI_TOOLS_DATE_TIME,
      
      .compile_datetime  = AI_TOOLS_COMPILE_TIME,
      
      .runtime_revision  = ai_platform_runtime_get_revision(),
      .runtime_version   = ai_platform_runtime_get_version(),

      .tool_revision     = AI_TOOLS_REVISION_ID,
      .tool_version      = {AI_TOOLS_VERSION_MAJOR, AI_TOOLS_VERSION_MINOR,
                            AI_TOOLS_VERSION_MICRO, 0x0},
      .tool_api_version  = AI_STRUCT_INIT,

      .api_version            = ai_platform_api_get_version(),
      .interface_api_version  = ai_platform_interface_api_get_version(),
      
      .n_macc            = 890,
      .n_inputs          = 0,
      .inputs            = NULL,
      .n_outputs         = 0,
      .outputs           = NULL,
      .params            = AI_STRUCT_INIT,
      .activations       = AI_STRUCT_INIT,
      .n_nodes           = 0,
      .signature         = 0x1b9c4e5c,
    };

    if (!ai_platform_api_get_network_report(network, &r)) return false;

    *report = r;
    return true;
  }
  return false;
}



AI_API_ENTRY
ai_bool ai_ae_model_get_report(
  ai_handle network, ai_network_report* report)
{
  ai_network* net_ctx = AI_NETWORK_ACQUIRE_CTX(network);

  if (report && net_ctx)
  {
    ai_network_report r = {
      .model_name        = AI_AE_MODEL_MODEL_NAME,
      .model_signature   = AI_AE_MODEL_MODEL_SIGNATURE,
      .model_datetime    = AI_TOOLS_DATE_TIME,
      
      .compile_datetime  = AI_TOOLS_COMPILE_TIME,
      
      .runtime_revision  = ai_platform_runtime_get_revision(),
      .runtime_version   = ai_platform_runtime_get_version(),

      .tool_revision     = AI_TOOLS_REVISION_ID,
      .tool_version      = {AI_TOOLS_VERSION_MAJOR, AI_TOOLS_VERSION_MINOR,
                            AI_TOOLS_VERSION_MICRO, 0x0},
      .tool_api_version  = AI_STRUCT_INIT,

      .api_version            = ai_platform_api_get_version(),
      .interface_api_version  = ai_platform_interface_api_get_version(),
      
      .n_macc            = 890,
      .n_inputs          = 0,
      .inputs            = NULL,
      .n_outputs         = 0,
      .outputs           = NULL,
      .map_signature     = AI_MAGIC_SIGNATURE,
      .map_weights       = AI_STRUCT_INIT,
      .map_activations   = AI_STRUCT_INIT,
      .n_nodes           = 0,
      .signature         = 0x1b9c4e5c,
    };

    if (!ai_platform_api_get_network_report(network, &r)) return false;

    *report = r;
    return true;
  }
  return false;
}


AI_API_ENTRY
ai_error ai_ae_model_get_error(ai_handle network)
{
  return ai_platform_network_get_error(network);
}


AI_API_ENTRY
ai_error ai_ae_model_create(
  ai_handle* network, const ai_buffer* network_config)
{
  return ai_platform_network_create(
    network, network_config, 
    AI_CONTEXT_OBJ(&AI_NET_OBJ_INSTANCE),
    AI_TOOLS_API_VERSION_MAJOR, AI_TOOLS_API_VERSION_MINOR, AI_TOOLS_API_VERSION_MICRO);
}


AI_API_ENTRY
ai_error ai_ae_model_create_and_init(
  ai_handle* network, const ai_handle activations[], const ai_handle weights[])
{
  ai_error err;
  ai_network_params params;

  err = ai_ae_model_create(network, AI_AE_MODEL_DATA_CONFIG);
  if (err.type != AI_ERROR_NONE) {
    return err;
  }
  
  if (ai_ae_model_data_params_get(&params) != true) {
    err = ai_ae_model_get_error(*network);
    return err;
  }
#if defined(AI_AE_MODEL_DATA_ACTIVATIONS_COUNT)
  /* set the addresses of the activations buffers */
  for (ai_u16 idx=0; activations && idx<params.map_activations.size; idx++) {
    AI_BUFFER_ARRAY_ITEM_SET_ADDRESS(&params.map_activations, idx, activations[idx]);
  }
#endif
#if defined(AI_AE_MODEL_DATA_WEIGHTS_COUNT)
  /* set the addresses of the weight buffers */
  for (ai_u16 idx=0; weights && idx<params.map_weights.size; idx++) {
    AI_BUFFER_ARRAY_ITEM_SET_ADDRESS(&params.map_weights, idx, weights[idx]);
  }
#endif
  if (ai_ae_model_init(*network, &params) != true) {
    err = ai_ae_model_get_error(*network);
  }
  return err;
}


AI_API_ENTRY
ai_buffer* ai_ae_model_inputs_get(ai_handle network, ai_u16 *n_buffer)
{
  if (network == AI_HANDLE_NULL) {
    network = (ai_handle)&AI_NET_OBJ_INSTANCE;
    AI_NETWORK_OBJ(network)->magic = AI_MAGIC_CONTEXT_TOKEN;
  }
  return ai_platform_inputs_get(network, n_buffer);
}


AI_API_ENTRY
ai_buffer* ai_ae_model_outputs_get(ai_handle network, ai_u16 *n_buffer)
{
  if (network == AI_HANDLE_NULL) {
    network = (ai_handle)&AI_NET_OBJ_INSTANCE;
    AI_NETWORK_OBJ(network)->magic = AI_MAGIC_CONTEXT_TOKEN;
  }
  return ai_platform_outputs_get(network, n_buffer);
}


AI_API_ENTRY
ai_handle ai_ae_model_destroy(ai_handle network)
{
  return ai_platform_network_destroy(network);
}


AI_API_ENTRY
ai_bool ai_ae_model_init(
  ai_handle network, const ai_network_params* params)
{
  ai_network* net_ctx = AI_NETWORK_OBJ(ai_platform_network_init(network, params));
  ai_bool ok = true;

  if (!net_ctx) return false;
  ok &= ae_model_configure_weights(net_ctx, params);
  ok &= ae_model_configure_activations(net_ctx, params);

  ok &= ai_platform_network_post_init(network);

  return ok;
}


AI_API_ENTRY
ai_i32 ai_ae_model_run(
  ai_handle network, const ai_buffer* input, ai_buffer* output)
{
  return ai_platform_network_process(network, input, output);
}


AI_API_ENTRY
ai_i32 ai_ae_model_forward(ai_handle network, const ai_buffer* input)
{
  return ai_platform_network_process(network, input, NULL);
}



#undef AI_AE_MODEL_MODEL_SIGNATURE
#undef AI_NET_OBJ_INSTANCE
#undef AI_TOOLS_DATE_TIME
#undef AI_TOOLS_COMPILE_TIME

