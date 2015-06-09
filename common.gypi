{
  'conditions': [
    ['bastian_engine == "v8"', {
      'defines': [
        'BASTIAN_V8'
      ],
      'include_dirs': [
        './deps/v8/include'
      ]
    }],
    ['bastian_engine == "jsc"', {
      'defines': [
        'BASTIAN_JSC'
      ],
    }],
    ['OS=="ios"', {
      'xcode_settings': {
        'SDKROOT': 'iphoneos',
        'TARGETED_DEVICE_FAMILY': '1,2',
        'CODE_SIGN_IDENTITY': 'iPhone Developer',
        'IPHONEOS_DEPLOYMENT_TARGET': '5.0',
        'ARCHS': '$(ARCHS_STANDARD_32_64_BIT)',
      },
    }],
    ['OS=="ios-simulator"', {
      'xcode_settings': {
        'SDKROOT': 'iphonesimulator',
        'CODE_SIGN_IDENTITY': 'iPhone Developer',
        'IPHONEOS_DEPLOYMENT_TARGET': '5.0',
        'GCC_VERSION': 'com.apple.compilers.llvm.clang.1_0',
        'ARCHS': '$(ARCHS_STANDARD_32_64_BIT)',
      },
    }],
  ]
}
