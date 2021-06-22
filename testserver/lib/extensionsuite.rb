
require 'fileutils'
require 'open3'
require 'tmpdir'

require 'json'

require 'utils.rb'

class ExtensionSuite

  public

  def initialize(test_cases_path, emulate_assemble_path)
    @test_cases_path       = File.absolute_path(test_cases_path)

    @emulate_assemble_path = File.absolute_path(emulate_assemble_path)

    @emulate  = File.absolute_path(File.join(emulate_assemble_path,"emulate"))
    @assemble = File.absolute_path(File.join(emulate_assemble_path,"assemble"))
    @extension = File.absolute_path(File.join(emulate_assemble_path, "dlc"))

    sources

  end

  # recomputes the sources and stores them
  def sources
    @sources = Dir.glob(File.join(@test_cases_path,"**","*.txt")).sort
    @sources
  end

  # takes a block argument which is called back by each result
  def run_tests(&block)

    @sources.each do |source|
      result = test_source(File.absolute_path(source))
      block.call({ :source => source, :result => result })
    end

  end

  def run_test(source, &block)
    if @sources.member?(source) then
      result = test_source(source)
      block.call({:source => source, :result => result})
    end
  end

  private

  def test_source(source)

    binary = source.sub(/\.s$/,"")
    extension_results = run_extension(source)

    { :source => source,
      :binary => binary,
      :extension => extension_results,
      :passed => true
    }
  end

  def run_extension(source)
    unless File.exists?(@extension) then
      return { :result => :error,
               :passed => false,
               :message => "extension binary cannot be found"
             }
    end

    Dir.mktmpdir do |dir|
      Dir.chdir(dir) do
        lex_results = Utils.run3("/usr/bin/timeout",
                                 ["--kill-after=5", "3", @extension, source],
                                 nil, 1024 * 1024 * 100)

                                 
        valgrind_results = Utils.run3("/usr/bin/valgrind",
                  ["--tool=memcheck", "--leak-check=full", @extension, source],
                    nil, 1024 * 1024 * 100)

        unless File.exist?("out.s") or File.exist?("registerMap.txt") then
          return { :result        => :ran,
                   :passed        => false,
                   :run_results   => lex_results,
                   :actual_machine => nil,
                   :actual_bin    => "<no output file produced>",
                 }
        end

        compiled_contents = File.open("out.s", "r") { |f| f.read } 

        reg_map = File.open("registerMap.txt") { |f| f.read }

        assemble_results = Utils.run3("/usr/bin/timeout",
                                  ["--kill-after=5", "3", @assemble, "out.s",
                                   "out"],
                                 nil, 1024 * 1024 * 100)     

        unless File.exist?("out.s") then
          return { :result        => :ran,
                    :passed        => false,
                    :run_results   => lex_results,
                    :actual_bin    => "<no output file produced>",
                  }
        end
                                 
        produced_contents = IO.popen(["/usr/bin/xxd", "-c4", "-g4", "out"]) { |io| io.read }

        emu_results = Utils.run3("/usr/bin/timeout",
                           ["--kill-after=5", "3", @emulate, "out"],
                           nil, 1024 * 1024 * 100)


        # passed = produced_contents == expected_contents &&
        #           run_results[:exit_status] == 0
        return {  :result       => :ran,
                  :passed       => true,
                  :run_results  => lex_results,
                  :ass_results    => assemble_results,
                  :emu_results    => emu_results,
                  :actual_machine => compiled_contents,
                  :actual_bin     => produced_contents,
                  :valgrind_results => valgrind_results,
                  :reg_map        => reg_map
                }

      end
    end


  end

end
