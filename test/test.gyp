{
  'targets': [
    {
      'target_name': 'test-bastian',
      'type': 'executable',
      'includes': [
        '../common.gypi',
      ],
      'include_dirs': [
        '../deps/gtest/include',
        '../include'
      ],
      'dependencies': [
        '../bastian.gyp:bastian'
      ],
      'libraries': [
        '../../deps/gtest/cbuild/libgtest.a',
        '../../deps/gtest/cbuild/libgtest_main.a',
      ],
      'sources': [
        './test-engine.cc',
        './test-fcontext.cc',
        './test-objcontext.cc',
        './test-value.cc',
      ],
      'conditions': [
        ['bastian_engine == "v8"', {
          'sources': [
            './v8/test-v8-common.cc',
          ],
        }],
        ['bastian_engine == "jsc"', {
          'sources': [
            './jsc/test-jsc-common.cc',
          ],
        }]
      ]

    }
  ]
}