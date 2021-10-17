defmodule CodeGen do
  @moduledoc """
  Documentation for `CodeGen`.
  生成 ast 和 stmt 的代码
  """

  defmodule TypeInfo do
    @enforce_keys [:name, :types, :visitor_return_types]
    defstruct [:name, :types, :visitor_return_types]
  end

  @typep sub_types :: %{String => [{String, String}]}

  @spec first_upper(String) :: String
  def first_upper(str) do
    first =
      str
      |> String.at(0)
      |> String.upcase()

    first <> String.slice(str, 1, String.length(str) - 1)
  end

  def gen_visitor_name() do
    "Visitor"
  end

  def gen_visit(type, ret) do
    "#{ret} Visit#{type}(#{type}&)"
  end

  def gen_header(file, name) do
    header = """
    #pragma once

    #include <string>
    #include <memory>
    #include "scanner.h"
    #include "literal.h"

    namespace #{name}{
    """

    IO.write(file, header)
  end

  @spec gen_visitor(File.io_device(), TypeInfo) :: nil
  def gen_visitor(file, %TypeInfo{types: types}) do
    visitor_name = gen_visitor_name()

    IO.puts(file, """
    template<typename R>
    struct #{visitor_name} {
        virtual ~#{visitor_name}() = default;
    """)

    types
    |> Enum.map(fn {type, _} ->
      IO.puts(file, "   virtual #{gen_visit(type, "R")} = 0;")
    end)

    IO.puts(file, "};")
  end

  def gen_base(file, %TypeInfo{name: name, visitor_return_types: retrurns}) do
    base_name = name |> first_upper()

    IO.puts(file, """
    struct #{base_name} {
        virtual ~#{base_name}() = default;
    """)

    Enum.map(retrurns, fn ret ->
      IO.puts(file, "    virtual #{ret} Accept(#{gen_visitor_name()}<#{ret}>&) = 0;")
    end)

    IO.puts(file, "};")
  end

  @spec gen_construct(File.io_device(), String, [{String, String}]) :: :ok
  def gen_construct(file, name, fields) do
    IO.write(file, "   explicit #{name}(")

    fields
    |> Enum.map(fn {type, name} -> "#{type} #{name}_" end)
    |> Enum.join(", ")
    |> (&IO.write(file, &1)).()

    if(Enum.empty?(fields)) do
      IO.write(file, ") noexcept")
    else
      IO.write(file, ") noexcept :")

      init_filed = fn {type, name} ->
        if(String.starts_with?(type, "std::unique_ptr")) do
          "#{name}(std::move(#{name}_))"
        else
          "#{name}(#{name}_)"
        end
      end

      fields
      |> Enum.map(init_filed)
      |> Enum.join(", ")
      |> (&IO.write(file, &1)).()
    end

    IO.puts(file, " {}")
  end

  @doc """
  type 是生成 对应子类的名字
  """
  def gen_accepts(ret, type) do
    """
        #{ret} Accept(Visitor<#{ret}>& visitor) override {
            return visitor.Visit#{type}(*this);
        }
    """
  end

  def gen_sub_type(file, sub_name, fields, %TypeInfo{
        visitor_return_types: rets,
        name: name
      }) do
    base = name |> first_upper()
    IO.puts(file, "struct #{sub_name}: public #{base} {")
    Enum.map(fields, fn {type, name} -> IO.puts(file, "   #{type} #{name};") end)
    gen_construct(file, sub_name, fields)

    rets
    |> Enum.map(fn ret -> gen_accepts(ret, sub_name) end)
    |> (&IO.puts(file, &1)).()

    IO.puts(file, "};")
    IO.puts(file, "")
  end

  @spec gen_code(TypeInfo) :: nil
  def gen_code(type_info) do
    {:ok, file} = File.open("#{type_info.name}.h", [:write])

    gen_header(file, type_info.name)
    Enum.map(type_info.types, fn {type, _} -> IO.puts(file, "struct #{type};") end)
    gen_visitor(file, type_info)
    gen_base(file, type_info)

    Enum.map(type_info.types, fn {name, fields} -> gen_sub_type(file, name, fields, type_info) end)

    IO.puts(file, "}")
  end

  @spec run :: none
  def run do
    expr = %TypeInfo{
      types: %{
        "Unary" => [{"Token", "op"}, {"std::unique_ptr<Expr>", "value"}],
        "String" => [{"std::string_view", "value"}],
        "Number" => [{"double", "value"}],
        "Bool" => [{"bool", "value"}],
        "Binary" => [
          {"std::unique_ptr<Expr>", "left"},
          {"Token", "op"},
          {"std::unique_ptr<Expr>", "right"}
        ],
        "Group" => [{"std::unique_ptr<Expr>", "expr"}],
        "Expression" => [{"std::unique_ptr<Expr>", "expr"}],
        "Print" => [{"std::unique_ptr<Expr>", "expr"}],
        "Nil" => []
      },
      name: "expr",
      visitor_return_types: ["Literal", "std::string"]
    }

    gen_code(expr)
  end
end
