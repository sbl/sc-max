install(TARGETS ${PROJECT_NAME} LIBRARY DESTINATION bruits)
install(FILES "${PROJECT_NAME}.sc" DESTINATION bruits)
install(
    FILES "${PROJECT_NAME}.schelp"
    DESTINATION bruits/HelpSource/Classes
    OPTIONAL
)

message("done")
