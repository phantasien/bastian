{
  'targets': [
    {
      'target_name': 'bastian',
      'type': 'static_library',
      'includes': [
        './common.gypi',
      ],
      'conditions': [
        ['bastian_engine == "v8"', {
          'dependencies': [
            './deps/v8/tools/gyp/v8.gyp:v8_libbase',
            './deps/v8/tools/gyp/v8.gyp:v8_snapshot',
            './deps/v8/tools/gyp/v8.gyp:v8_libplatform',
            './deps/v8/tools/gyp/v8.gyp:v8_base'
          ]
        }],
        ['bastian_engine == "jsc"', {
          'link_settings': {
            'libraries': [
              '$(SDKROOT)/System/Library/Frameworks/JavascriptCore.framework',
            ],
          },
        }]
      ],
      'sources': [
        'src/engine.cc',
        'src/fcontext.cc',
        'src/objcontext.cc',
        'src/value.cc'
      ]
    }
  ]
}