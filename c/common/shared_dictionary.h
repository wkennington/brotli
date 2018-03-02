#ifndef BROTLI_COMMON_SHARED_DICTIONARY_H_
#define BROTLI_COMMON_SHARED_DICTIONARY_H_

/* TODO: cut this dependency, if possible */
#include "./dictionary.h"
#include <brotli/port.h>
/* TODO: cut this dependency, if possible */
#include "./transform.h"
#include <brotli/types.h>

#if defined(__cplusplus) || defined(c_plusplus)
extern "C" {
#endif

#define SHARED_BROTLI_MIN_DICTIONARY_WORD_LENGTH 4
#define SHARED_BROTLI_MAX_DICTIONARY_WORD_LENGTH 31
#define SHARED_BROTLI_NUM_DICTIONARY_CONTEXTS 64
#define SHARED_BROTLI_MAX_COMPOUND_DICTS 15

typedef struct BrotliSharedDictionary {
  /* LZ77 prefixes (compound dictionary). */
  uint32_t num_prefix;  /* max SHARED_BROTLI_MAX_COMPOUND_DICTS */
  size_t prefix_size[SHARED_BROTLI_MAX_COMPOUND_DICTS];
  const uint8_t* prefix[SHARED_BROTLI_MAX_COMPOUND_DICTS];

  /* If set, the context map is used to select word and transform list from 64
     contexts, if not set, the context map is not used and only words[0] and
     transforms[0] are to be used. */
  BROTLI_BOOL context_based;

  uint8_t context_map[SHARED_BROTLI_NUM_DICTIONARY_CONTEXTS];

  /* Amount of word_list+transform_list combinations. */
  uint8_t num_dictionaries;

  /* Must use num_dictionaries values. */
  const BrotliDictionary* words[SHARED_BROTLI_NUM_DICTIONARY_CONTEXTS];

  /* Must use num_dictionaries values. */
  const BrotliTransforms* transforms[SHARED_BROTLI_NUM_DICTIONARY_CONTEXTS];

  /* Amount of custom word lists. May be 0 if only Brotli's built-in is used */
  uint8_t num_word_lists;

  /* Contents of the custom words lists. Must be NULL if num_word_lists is 0. */
  BrotliDictionary* words_instances;

  /* Amount of custom transform lists. May be 0 if only Brotli's built-in is
     used */
  uint8_t num_transform_lists;

  /* Contents of the custom transform lists. Must be NULL if num_transform_lists
     is 0. */
  BrotliTransforms* transforms_instances;

  /* Concatenated prefix_suffix_maps of the custom transform lists. Must be NULL
     if num_transform_lists is 0. */
  uint16_t* prefix_suffix_maps;

  /* Memory management */
  brotli_alloc_func alloc_func;
  brotli_free_func free_func;
  void* memory_manager_opaque;
} BrotliSharedDictionary;

typedef enum BrotliSharedDictionaryType {
  /* A generic raw file as prefix (compound) dictionary. */
  BROTLI_SHARED_DICTIONARY_RAW = 0,
  /* A file in the shared dictionary format, can replace words and/or contain
     multiple compound dictionaries. */
  BROTLI_SHARED_DICTIONARY_SERIALIZED = 1
} BrotliSharedDictionaryType;

/* Initializes a dictionary from a shared dictionary file or a raw data file. */
/* Set alloc_func and free_func to null to use default memory allocation
   functions. Opaque will be passed to custom allocation functions. */
BROTLI_COMMON_API BROTLI_BOOL BrotliSharedDictionaryInit(
    BrotliSharedDictionary* dict, BrotliSharedDictionaryType type,
    const uint8_t* data, size_t size,
    brotli_alloc_func alloc_func, brotli_free_func free_func, void* opaque);

/* Frees allocated memory in the dictionary, but not the dictionary itself */
BROTLI_COMMON_API void BrotliSharedDictionaryCleanup(
    BrotliSharedDictionary* dict);

/* Initializes a default dictionary, using Brotli's built-in dictionary. */
/* Set alloc_func and free_func to null to use default memory allocation
   functions. Opaque will be passed to custom allocation functions. */
BROTLI_COMMON_API void BrotliSharedDictionaryInitDefault(
    BrotliSharedDictionary* dict,
    brotli_alloc_func alloc_func, brotli_free_func free_func, void* opaque);

/* Attaches one dictionary to another, to combine compound dictionaries. */
BROTLI_COMMON_API BROTLI_BOOL BrotliSharedDictionaryAttach(
    BrotliSharedDictionary* dict, BrotliSharedDictionaryType type,
    const uint8_t* data, size_t size);

#if defined(__cplusplus) || defined(c_plusplus)
}  /* extern "C" */
#endif

#endif  /* BROTLI_COMMON_SHARED_DICTIONARY_H_ */
