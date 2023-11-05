if(NOT PX_SHADERC_BIN)
  # Look for shaderc
  if (EXISTS "/usr/bin/shaderc")
    set(PX_SHADERC_BIN "/usr/bin/shaderc")
  elseif(EXISTS "${PROJECT_SOURCE_DIR}/shaderc")
    set(PX_SHADERC_BIN "${PROJECT_SOURCE_DIR}/shaderc")
  elseif(EXISTS "${PROJECT_SOURCE_DIR}/tools/shaderc")
    set(PX_SHADERC_BIN "${PROJECT_SOURCE_DIR}/tools/shaderc")
  endif()
endif()

function(_bgfx_shaderc_compile ARG_OUT)
  cmake_parse_arguments(
      ARG
      "DEPENDS;ANDROID;ASM_JS;IOS;LINUX;NACL;OSX;WINDOWS;PREPROCESS;RAW;FRAGMENT;VERTEX;COMPUTE;VERBOSE;DEBUG;DISASM;WERROR"
      "FILE;OUTPUT;VARYINGDEF;BIN2C;PROFILE;O"
      "INCLUDES;DEFINES"
      ${ARGN}
  )
  set(CLI "")

  # -f
  if(ARG_FILE)
    list(APPEND CLI "-f" "${ARG_FILE}")
  endif()

  # -i
  if(ARG_INCLUDES)
    foreach(INCLUDE ${ARG_INCLUDES})
      list(APPEND CLI "-i")
      list(APPEND CLI "${INCLUDE}")
    endforeach()
  endif()

  # -o
  if(ARG_OUTPUT)
    list(APPEND CLI "-o" "${ARG_OUTPUT}")
  endif()

  # --bin2c
  if(ARG_BIN2C)
    list(APPEND CLI "--bin2c" "${ARG_BIN2C}")
  endif()

  # --depends
  if(ARG_DEPENDS)
    list(APPEND CLI "--depends")
  endif()

  # --platform
  set(PLATFORM "")
  set(PLATFORMS "ANDROID;ASM_JS;IOS;LINUX;NACL;OSX;WINDOWS")
  foreach(P ${PLATFORMS})
    if(ARG_${P})
      if(PLATFORM)
        message(SEND_ERROR "Call to _bgfx_shaderc_parse() cannot have both flags ${PLATFORM} and ${P}.")
        return()
      endif()
      set(PLATFORM "${P}")
    endif()
  endforeach()
  if(PLATFORM STREQUAL "")
    message(SEND_ERROR "Call to _bgfx_shaderc_parse() must have a platform flag: ${PLATFORMS}")
    return()
  elseif(PLATFORM STREQUAL ANDROID)
    list(APPEND CLI "--platform" "android")
  elseif(PLATFORM STREQUAL ASM_JS)
    list(APPEND CLI "--platform" "asm.js")
  elseif(PLATFORM STREQUAL IOS)
    list(APPEND CLI "--platform" "ios")
  elseif(PLATFORM STREQUAL NACL)
    list(APPEND CLI "--platform" "nacl")
  elseif(PLATFORM STREQUAL OSX)
    list(APPEND CLI "--platform" "osx")
  elseif(PLATFORM STREQUAL UNIX)
    list(APPEND CLI "--platform" "linux")
  elseif(PLATFORM STREQUAL WINDOWS)
    list(APPEND CLI "--platform" "windows")
  endif()

  # --preprocess
  if(ARG_PREPROCESS)
    list(APPEND CLI "--preprocess")
  endif()

  # --define
  if(ARG_DEFINES)
    list(APPEND CLI "--defines")
    set(DEFINES "")
    foreach(DEFINE ${ARG_DEFINES})
      if(NOT "${DEFINES}" STREQUAL "")
        set(DEFINES "${DEFINES}\\\\;${DEFINE}")
      else()
        set(DEFINES "${DEFINE}")
      endif()
    endforeach()
    list(APPEND CLI "${DEFINES}")
  endif()

  # --raw
  if(ARG_RAW)
    list(APPEND CLI "--raw")
  endif()

  # --type
  set(TYPE "")
  set(TYPES "FRAGMENT;VERTEX;COMPUTE")
  foreach(T ${TYPES})
    if(ARG_${T})
      if(TYPE)
        message(SEND_ERROR "Call to _bgfx_shaderc_compile() cannot have both flags ${TYPE} and ${T}.")
        return()
      endif()
      set(TYPE "${T}")
    endif()
  endforeach()
  if("${TYPE}" STREQUAL "")
    message(SEND_ERROR "Call to _bgfx_shaderc_compile() must have a type flag: ${TYPES}")
    return()
  elseif("${TYPE}" STREQUAL "FRAGMENT")
    list(APPEND CLI "--type" "fragment")
  elseif("${TYPE}" STREQUAL "VERTEX")
    list(APPEND CLI "--type" "vertex")
  elseif("${TYPE}" STREQUAL "COMPUTE")
    list(APPEND CLI "--type" "compute")
  endif()

  # --varyingdef
  if(ARG_VARYINGDEF)
    list(APPEND CLI "--varyingdef" "${ARG_VARYINGDEF}")
  endif()

  # --verbose
  if(ARG_VERBOSE)
    list(APPEND CLI "--verbose")
  endif()

  # --debug
  if(ARG_DEBUG)
    list(APPEND CLI "--debug")
  endif()

  # --disasm
  if(ARG_DISASM)
    list(APPEND CLI "--disasm")
  endif()

  # --profile
  if(ARG_PROFILE)
    list(APPEND CLI "--profile" "${ARG_PROFILE}")
  endif()

  # -O
  if(ARG_O)
    list(APPEND CLI "-O" "${ARG_O}")
  endif()

  # --Werror
  if(ARG_WERROR)
    list(APPEND CLI "--Werror")
  endif()

  set(${ARG_OUT} ${CLI} PARENT_SCOPE)
endfunction(_bgfx_shaderc_compile)

function(add_bgfx_shader FILE OUTPUT_FOLDER)
  # Check shaderc
  if (NOT PX_SHADERC_BIN)
    message(SEND_ERROR "shaderc is not found")
  endif()

  get_filename_component(FILE "${FILE}" ABSOLUTE)
  get_filename_component(FILENAME "${FILE}" NAME_WE)
  string(SUBSTRING "${FILENAME}" 0 2 TYPE)
  if("${TYPE}" STREQUAL "fs")
    set(TYPE "FRAGMENT")
  elseif("${TYPE}" STREQUAL "vs")
    set(TYPE "VERTEX")
  elseif("${TYPE}" STREQUAL "cs")
    set(TYPE "COMPUTE")
  else()
    set(TYPE "")
  endif()

  if(NOT "${TYPE}" STREQUAL "")
    set(COMMON FILE ${FILE} ${TYPE}) #INCLUDES ${BGFX_DIR}/src)
    set(OUTPUTS "")
    set(OUTPUTS_PRETTY "")

    if(WIN32)
      # dx9
      if(NOT "${TYPE}" STREQUAL "COMPUTE")
        set(DX9_OUTPUT ${OUTPUT_FOLDER}/dx9/${FILENAME}.bin)
        _bgfx_shaderc_compile(
            DX9 ${COMMON} WINDOWS
            PROFILE s_3_0
            O 3
            OUTPUT ${DX9_OUTPUT}
        )
        list(APPEND OUTPUTS "DX9")
        set(OUTPUTS_PRETTY "${OUTPUTS_PRETTY}DX9, ")
      endif()

      # dx11
      set(DX11_OUTPUT ${OUTPUT_FOLDER}/dx11/${FILENAME}.bin)
      if(NOT "${TYPE}" STREQUAL "COMPUTE")
        _bgfx_shaderc_compile(
            DX11 ${COMMON} WINDOWS
            PROFILE s_5_0
            O 3
            OUTPUT ${DX11_OUTPUT}
        )
      else()
        _bgfx_shaderc_compile(
            DX11 ${COMMON} WINDOWS
            PROFILE s_5_0
            O 1
            OUTPUT ${DX11_OUTPUT}
        )
      endif()
      list(APPEND OUTPUTS "DX11")
      set(OUTPUTS_PRETTY "${OUTPUTS_PRETTY}DX11, ")
    endif()

    if(APPLE)
      # metal
      set(METAL_OUTPUT ${OUTPUT_FOLDER}/metal/${FILENAME}.bin)
      _bgfx_shaderc_compile(METAL ${COMMON} OSX PROFILE metal OUTPUT ${METAL_OUTPUT})
      list(APPEND OUTPUTS "METAL")
      set(OUTPUTS_PRETTY "${OUTPUTS_PRETTY}Metal, ")
    endif()

    # glsl
    set(GLSL_OUTPUT ${OUTPUT_FOLDER}/glsl/${FILENAME}.bin)
    if(NOT "${TYPE}" STREQUAL "COMPUTE")
      _bgfx_shaderc_compile(GLSL ${COMMON} LINUX PROFILE 140 OUTPUT ${GLSL_OUTPUT})
    else()
      _bgfx_shaderc_compile(GLSL ${COMMON} LINUX PROFILE 430 OUTPUT ${GLSL_OUTPUT})
    endif()
    list(APPEND OUTPUTS "GLSL")
    set(OUTPUTS_PRETTY "${OUTPUTS_PRETTY}GLSL, ")

    # spirv
    if(NOT "${TYPE}" STREQUAL "COMPUTE")
      set(SPIRV_OUTPUT ${OUTPUT_FOLDER}/spirv/${FILENAME}.bin)
      _bgfx_shaderc_compile(SPIRV ${COMMON} LINUX PROFILE spirv OUTPUT ${SPIRV_OUTPUT})
      list(APPEND OUTPUTS "SPIRV")
      set(OUTPUTS_PRETTY "${OUTPUTS_PRETTY}SPIRV")
      set(OUTPUT_FILES "")
      set(COMMANDS "")
    endif()

    foreach(OUT ${OUTPUTS})
      list(APPEND OUTPUT_FILES ${${OUT}_OUTPUT})
      list(APPEND COMMANDS COMMAND ${PX_SHADERC_BIN} ${${OUT}})
      get_filename_component(OUT_DIR ${${OUT}_OUTPUT} DIRECTORY)
      file(MAKE_DIRECTORY ${OUT_DIR})
    endforeach()

#    file(RELATIVE_PATH PRINT_NAME ${PROJECT_SOURCE_DIR} ${FILE})
    set(PRINT_NAME ${FILE})
    add_custom_command(
        MAIN_DEPENDENCY ${FILE} OUTPUT ${OUTPUT_FILES} ${COMMANDS}
        COMMENT "Compiling shader ${PRINT_NAME} for ${OUTPUTS_PRETTY}"
    )
  endif()

endfunction(add_bgfx_shader)