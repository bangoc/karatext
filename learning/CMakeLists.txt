add_executable(capi capi.c)
add_executable(text text.cc)
add_executable(textcenter textcenter.cc)
add_executable(twosent twosent.cc)
add_executable(twostyle twostyle.cc)
add_executable(frame frame.cc)
foreach (p capi text textcenter twosent twostyle frame)
  target_link_libraries(${p} blend2d)
endforeach()