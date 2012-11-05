{
  "targets": [
    {
      "target_name": "egl",
      "sources": [
        "src/egl.cc"
      ],
      "conditions": [
        [
          "OS=='linux'",
          {
            "conditions": [
              [
                "target_arch=='arm'",
                {
                  "link_settings": {
                    "libraries": [
                      "-L/opt/vc/lib",
                      "-lGLESv2",
                      "-lEGL",
                      "-lbcm_host"
                    ]
                  }
                },
                {
                  "link_settings": {
                    "libraries": [
                      "-lEGL"
                    ]
                  }
                }
              ]
            ]
          }
        ]
      ]
    }
  ]
}