# sc-max build-helper

task :default => :release

desc "compile all externals, copy helpfiles, zip it up"
task :release => [:compile_xcode, :copy_helpfiles] do
  v = File.open("VERSION", "r"){|f| f.read}.chomp

  sh "cp COPYING _build/"
  sh "cp README _build/"
  sh "zip -r sc-max-#{v}.zip _build"
end

desc "compile all projects that start with sc.*"
task :compile_xcode do
  puts "-------------------"
  puts "COMPILING EXTERNALS"
  puts "-------------------"
  
  Dir.glob("sc.*/*.xcodeproj").each do |path|
    sh "xcodebuild -project #{path} -target 'max-external' -configuration 'Deployment' build"
  end  
end

desc "copy over all helpfiles from the sc.* projects"
task :copy_helpfiles  do
  puts "-----------------"
  puts "COPYING HELPFILES"
  puts "-----------------"

  Dir.glob("sc.*/*.maxhelp").each do |maxhelp|
    sh "cp #{maxhelp} _build/"
  end
  
end

desc "remove mxo's and maxhelp from _build"
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
  
  sh "rm sc-max*.zip"
end
