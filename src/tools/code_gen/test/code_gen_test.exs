defmodule CodeGenTest do
  use ExUnit.Case
  doctest CodeGen

  test "greets the world" do
    assert CodeGen.run()
  end
end
