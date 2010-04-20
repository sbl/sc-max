# sc-max build-helper

task :default => :release


task :release => [:clean, :compile_xcode, :copy_helpfiles] do
  sh "zip -r _build sc-max.zip"
end

task :compile_xcode do
  puts "-------------------"
  puts "COMPILING EXTERNALS"
  puts "-------------------"

  
  Dir.glob("sc.*/*.xcodeproj").each do |path|
    sh "xcodebuild -project #{path} -target 'max-external' -configuration 'Deployment' build"
  end  
end

task :copy_helpfiles  do
  puts "-----------------"
  puts "COPYING HELPFILES"
  puts "-----------------"

  Dir.glob("sc.*/*.maxhelp").each do |maxhelp|
    sh "cp #{maxhelp} _build/"
  end
  
end

task :clean do
  puts "-----------"
  puts "CLEANING UP"
  puts "-----------"

  Dir.glob("sc.*/*.xcodeproj").each do |path|
    sh "xcodebuild -project #{path} clean"
  end

  Dir.glob("_build/*.maxhelp").each do |maxhelp|
    sh "rm #{maxhelp}"
  end
end
