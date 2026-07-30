/***************************************************************
 * Copyright (C) 2026
 *    Computer Graphics Support Group of 30 Phys-Math Lyceum
 ***************************************************************/

/* FILE NAME   : tam_json.h
 * PURPOSE     : Tough Arrangement Manager project.
 *               JSON declaration file.
 * PROGRAMMER  : CGSG'Sr'2025.
 *               Kupchenkov Ivan (IK4),
 *               Novozhilova Maryana (MN7).
 * LAST UPDATE : 22.04.2026
 * NOTE        : Namespace 'tam'.
 *
 * No part of this file may be changed without agreement of
 * Computer Graphics Support Group of 30 Phys-Math Lyceum
 */

#ifndef __tam_json_h_
#define __tam_json_h_

#include "tam_def.h"

/* 'tam' namespace */
namespace tam
{
  /* JSON structure forward declaration */
  struct json;
  /* JSON array type declaration */
  using json_array = std::vector<json>;
  /* JSON object type declaration */
  using json_object = std::map<std::string, json>;

  /* JSON structure */
  struct json :
    public std::variant<std::nullptr_t,
                        bool,
                        INT,
                        DOUBLE,
                        std::string,
                        json_array,
                        json_object>
  {
    using variant::variant;

    /* Access to json object field function.
     * ARGUMENTS:
     *   - object field index name:
     *       const std::string &FieldName;
     * RETURNS:
     *   (const json &) reference to found object.
     */
    const json & operator[]( const std::string &FieldName ) const
    {
      static const json Empty {};
      if (!std::holds_alternative<json_object>(*this))
        return Empty;
      auto &object = std::get<json_object>(*this);
      auto fld = object.find(FieldName);
      if (fld == object.end())
        return Empty;
      return fld->second;
    } /* End of 'operator[]' function */

    /* Access to json array element function.
     * ARGUMENTS:
     *   - array index:
     *       INT Index;
     * RETURNS:
     *   (const json &) reference to found object.
     */
    const json & operator[]( INT Index ) const
    {
      static const json Empty {};
      if (!std::holds_alternative<json_object>(*this))
        return Empty;
      auto &array = std::get<json_array>(*this);
      if (Index < 0 || Index >= array.size())
        return Empty;
      return array[Index];
    } /* End of 'operator[]' function */
 
    /* Get JSON value as string function.
     * ARGUMENTS:
     *   - default string:
     *       const std::string &S;
     * RETURNS:
     *   (const std::string &) reference to string.
     */
    const std::string & GetString( const std::string &S = "" ) const
    {
      if (std::holds_alternative<std::string>(*this))
        return std::get<std::string>(*this);
      return S;
    } /* End of 'GetString' function */

    /* Get JSON value as array function.
     * ARGUMENTS: None.
     * RETURNS:
     *   (const json_array &) reference to array.
     */
    const json_array & GetArray( VOID ) const
    {
      if (std::holds_alternative<json_array>(*this))
        return std::get<json_array>(*this);
      static const json_array EmptyArray {};
      return EmptyArray;
    } /* End of 'GetArray' function */

    /* Get JSON value as object function.
     * ARGUMENTS: None.
     * RETURNS:
     *   (const json_object &) reference to object.
     */
    const json_object & GetJsObject( VOID ) const
    {
      if (std::holds_alternative<json_object>(*this))
        return std::get<json_object>(*this);
      static const json_object EmptyObject {};
      return EmptyObject;
    } /* End of 'GetObject' function */

    /* Get JSON value as double function.
     * ARGUMENTS:
     *   - default value to be set:
     *       DBL DefaultValue;
     * RETURNS:
     *   (DBL) result double value.
     */
    DBL GetDouble( DBL DefaultValue = 0.0 ) const
    {
      if (std::holds_alternative<DBL>(*this))
        return std::get<DBL>(*this);
      else if (std::holds_alternative<INT>(*this))
        return std::get<INT>(*this);
      return DefaultValue;
    } /* End of 'GetDouble' function */

    /* Get JSON value as integer function.
     * ARGUMENTS:
     *   - default integer value:
     *       INT DefaultInt;
     * RETURNS:
     *   (INT) result integer value.
     */
    INT GetInt( INT DefaultInt = 0 ) const
    {
      if (std::holds_alternative<INT>(*this))
        return std::get<INT>(*this);
      else if (std::holds_alternative<DBL>(*this))
        return (INT)std::get<DBL>(*this);
      return DefaultInt;
    } /* End of 'GetInt' function */

    /* Get JSON value as bool function.
     * ARGUMENTS:
     *   - default bool value:
     *       bool DefaultInt;
     * RETURNS:
     *   (bool) result integer value.
     */
    bool GetBool( bool DefaultBool = FALSE ) const
    {
      if (std::holds_alternative<bool>(*this))
        return std::get<bool>(*this);
      return DefaultBool;
    } /* End of 'GetInt' function */
  }; /* End of 'json' structure */

  /* JSON visitor strcture */
  struct json_visitor
  {
    /* Operator() function.
     * ARGUMENTS:
     *   - json struct variable:
     *       std::nullptr_t N;
     * RETURNS: None.
     */
    VOID operator()( const std::nullptr_t ) const
    {
      tam::logger::line >> 0xFF0000 << "null" >> tam::logger::DefFore;
    } /* End of 'operator()' function */

    /* Operator() function.
     * ARGUMENTS:
     *   - json struct variable:
     *       BOOL F;
     * RETURNS: None.
     */
    VOID operator()( const bool F ) const
    {
      tam::logger::line >> 0xFFFF00 << "bool: " << F >> tam::logger::DefFore;
    } /* End of 'operator()' function */

    /* Operator() function.
     * ARGUMENTS:
     *   - json struct variable:
     *       INT X;
     * RETURNS: None.
     */
    VOID operator()( const INT X ) const
    {
      tam::logger::line >> 0xAA88FF << "int: " << X >> tam::logger::DefFore;
    } /* End of 'operator()' function */

    /* Operator() function.
     * ARGUMENTS:
     *   - json struct variable:
     *       DOUBLE X;
     * RETURNS: None.
     */
    VOID operator()( const DOUBLE X ) const
    {
      tam::logger::line >> 0x30FF30 << "double: " << X >> tam::logger::DefFore;
    } /* End of 'operator()' function */

    /* Operator() function.
     * ARGUMENTS:
     *   - json struct variable:
     *       std::string S;
     * RETURNS: None.
     */
    VOID operator()( const std::string S ) const
    {
      if (S.length() >= 102)
        tam::logger::line >> 0xFF0000 << "string: ..." >> tam::logger::DefFore;
      else
        tam::logger::line >> 0x00FFFF << "string: " >> 0xFFAA88 << S >> tam::logger::DefFore;
    } /* End of 'operator()' function */

    /* Operator() function.
     * ARGUMENTS:
     *   - json struct variable:
     *       json_array A;
     * RETURNS: None.
     */
    VOID operator()( const json_array A ) const
    {
      tam::logger::line >> 0xFF3347 << "array: [" >> tam::logger::DefFore;
      INT i = 0;
      for (auto &val : A)
      {
        i++;
        tam::logger::line >> 0xFF3347 << "|" >> tam::logger::DefFore;
        std::visit(json_visitor{}, val);
        if (i >= A.size())
          tam::logger::line << "|";
        else
          tam::logger::line << "|, ";
      }
      tam::logger::line >> 0xFF3347 << "]" >> tam::logger::DefFore;
    } /* End of 'operator()' function */

    /* Operator() function.
     * ARGUMENTS:
     *   - json struct variable:
     *       json_object Obj;
     * RETURNS: None.
     */
    VOID operator()( const json_object Obj ) const
    {
      tam::logger::line >> 0x3047FF << "object: {" >> tam::logger::DefFore;
      INT i = 0;
      for (auto &[key, val] : Obj)
      {
        i++;
        tam::logger::line >> 0xFFAA88 << "<\"" << key << "\": " >> tam::logger::DefFore;
        std::visit(json_visitor{}, val);
        if (i >= Obj.size())
          tam::logger::line << ">";
        else
          tam::logger::line << ">, ";
      }
      tam::logger::line >> 0x3047FF << "}" >> tam::logger::DefFore;
    } /* End of 'operator()' function */
  }; /* End of 'json_visitor' structure */

  /* 'json_saver' handle class */
  class json_saver
  {
  private:
    std::fstream File;

    /* Put spaces into json file function.
     * ARGUMENTS:
     *   - level for spaces:
     *       INT Level;
     * RETURNS: None.
     */
    VOID WriteSpaces( INT Level )
    {
      for (INT i = 0; i < Level; i++)
        File << "  ";
    } /* End of 'WriteSpaces' function */

    /* Write bool to json file function.
     * ARGUMENTS:
     *   - bool to write:
     *       bool B;
     * RETURNS: None.
     */
    VOID WriteBOOL( bool B )
    {
      if (B)
        File << "true";
      else
        File << "false";
    } /* End of 'WriteBOOL' function */

    /* Write int to json file function.
     * ARGUMENTS:
     *   - int to write:
     *       INT I;
     * RETURNS: None.
     */
    VOID WriteINT( INT I )
    {
      File << I;
    } /* End of 'WriteINT' function */

    /* Write double to json file function.
     * ARGUMENTS:
     *   - double to write:
     *       DBL D;
     * RETURNS: None.
     */
    VOID WriteDBL( DBL D )
    {
      File << D;
    } /* End of 'WriteDBL' function */

    /* Write string to json file function.
     * ARGUMENTS:
     *   - string to write:
     *       const std::string S;
     * RETURNS: None.
     */
    VOID WriteString( const std::string S )
    {
      File << "\"" << S << "\"";
    } /* End of 'WriteString' function */

    /* Write json array to json file function.
     * ARGUMENTS:
     *   - json array to write:
     *       const json_array Arr;
     *   - level of writing:
     *       INT Level;
     * RETURNS: None.
     */
    VOID WriteArray( const json_array Arr, INT Level = 0 )
    {
      File << "[\n";
      INT i = 0;
      for (auto &val : Arr)
      {
        i++;
        WriteSpaces(Level + 1);
        if (std::holds_alternative<bool>(val))
          WriteBOOL(val.GetBool());
        else if (std::holds_alternative<INT>(val))
          WriteINT(val.GetInt());
        else if (std::holds_alternative<DBL>(val))
          WriteDBL(val.GetDouble());
        else if (std::holds_alternative<std::string>(val))
          WriteString(val.GetString());
        else if (std::holds_alternative<json_array>(val))
          WriteArray(val.GetArray(), Level + 1);
        else if (std::holds_alternative<json_object>(val))
          WriteJsObject(val.GetJsObject(), Level + 1);
        if (i >= Arr.size())
          File << "\n";
        else
          File << ",\n";
      }
      WriteSpaces(Level);
      File << "]";
    } /* End of 'WriteArray' function */

    /* Write json object to json file function.
     * ARGUMENTS:
     *   - json object to write:
     *       const json_object Obj;
     *   - level of writing:
     *       INT Level;
     * RETURNS: None.
     */
    VOID WriteJsObject( const json_object Obj, INT Level = 0 )
    {
      File << "{\n";
      INT i = 0;
      for (auto &[key, val] : Obj)
      {
        i++;
        WriteSpaces(Level + 1);
        File << "\"" << key << "\": ";
        if (std::holds_alternative<bool>(val))
          WriteBOOL(val.GetBool());
        else if (std::holds_alternative<INT>(val))
          WriteINT(val.GetInt());
        else if (std::holds_alternative<DBL>(val))
          WriteDBL(val.GetDouble());
        else if (std::holds_alternative<std::string>(val))
          WriteString(val.GetString());
        else if (std::holds_alternative<json_array>(val))
          WriteArray(val.GetArray(), Level + 1);
        else if (std::holds_alternative<json_object>(val))
          WriteJsObject(val.GetJsObject(), Level + 1);
        if (i >= Obj.size())
          File << "\n";
        else
          File << ",\n";
      }
      WriteSpaces(Level);
      File << "}";
    } /* End of 'WriteObject' function */

  public:
    /* Class default constructor */
    json_saver( VOID )
    {
    } /* End of 'json_saver' function */

    /* Class default constructor
     * ARGUMENTS:
     *   - file name:
     *       std::string FileName;
     */
    json_saver( std::string FileName )
    {
      File.open(FileName, std::fstream::out);
    } /* End of 'json_saver' function */

    /* Class default destructor.
     * ARGUMENTS: None.
     * RETURNS: None.
     */
    ~json_saver( VOID )
    {
      File.close();
    } /* End of '~json_saver' function */

    /* Save json into file function.
     * ARGUMENTS:
     *   - json:
     *       json Json;
     * RETURNS: None.
     */
    VOID Save( json Json )
    {
      if (std::holds_alternative<BOOL>(Json))
        WriteBOOL(Json.GetBool());
      else if (std::holds_alternative<INT>(Json))
        WriteINT(Json.GetInt());
      else if (std::holds_alternative<DBL>(Json))
        WriteDBL(Json.GetDouble());
      else if (std::holds_alternative<std::string>(Json))
        WriteString(Json.GetString());
      else if (std::holds_alternative<json_array>(Json))
        WriteArray(Json.GetArray(), 0);
      else if (std::holds_alternative<json_object>(Json))
        WriteJsObject(Json.GetJsObject(), 0);
    } /* End of 'Save' function */
  }; /* End of 'json_saver' class */

  /* 'json_loader' handle class */
  class json_loader
  {
  private:
    std::string File {}; // File string
    size_t Flen {};      // File length
    size_t Pos {};       // Current position in file

    /* Skip white spaces function.
     * ARGUMENTS: None.
     * RETURNS: None.
     */
    VOID SkipWhiteSpace( VOID )
    {
      while (Pos < Flen && std::isspace(static_cast<UCHAR>(File[Pos])))
        Pos++;
    } /* End of 'SkipWhiteSpace' function */

    /* Skip commentaries function.
     * ARGUMENTS: None.
     * RETURNS: None.
     */
    VOID SkipComment( VOID )
    {
      if (Pos + 1 < Flen && File[Pos] == '/')
      {
        if (File[Pos + 1] == '/')
          while (Pos < Flen && File[Pos] != '\n')
            Pos++;
        else if (File[Pos + 1] == '*')
        {
          Pos += 2;
           while (Pos + 1 < Flen &&
                  !(File[Pos] == '*' && File[Pos + 1] == '/'))
             Pos++;
           if (Pos + 1 < Flen)
             Pos += 2;
        }
      }
    } /* End of 'SkipComment' function */

    /* Parse value function.
     * ARGUMENTS: None.
     * RETURNS:
     *   (json) parsed result.
     */
    json ParseValue( VOID )
    {
      SkipWhiteSpace();
      SkipComment();

      switch (File[Pos])
      {
      case '"':
        return ParseString();
      case '{':
        return ParseObject();
      case '[':
        return ParseArray();
      case '0':
      case '1':
      case '2':
      case '3':
      case '4':
      case '5':
      case '6':
      case '7':
      case '8':
      case '9':
      case '-':
        return ParseNumber();
      case 't':
        Pos += 4;
        return json{TRUE};
      case 'f':
        Pos += 5;
        return json{FALSE};
      case 'i':
        if (Pos + 2 < Flen && File[Pos + 1] == 'n' && File[Pos + 2] == 'f')
        {
          Pos += 3;
          return json{INFINITY};
        }
        else
          return json{nullptr};
      default:
        return json {nullptr};
      }
    } /* End of 'ParseValue' function */

    /* Parse number function.
     * ARGUMENTS: None.
     * RETURNS:
     *   (json) parsed number.
     */
    json ParseNumber( VOID )
    {
      CHAR *err;

      DBL x = std::strtod(&File[Pos], &err);
      if (err == nullptr)
        return json{0.0};
      Pos += err - &File[Pos];
      return json{x};
      /*
      INT Res = 0, cnt = 0, sign = 1, deg = 0, deg_sign = 1;
      BOOL IsDBL = FALSE;
      DBL ResDBL = 0, multDBl = 0.1;

      if (Pos < Flen)
        if (File[Pos] == '-')
          sign = -1, Pos++;
      while (Pos < Flen)
      {
        if (File[Pos] == '.')
        {
          IsDBL = TRUE;
          ResDBL = static_cast<DBL>(Res);
          Pos++;
          continue;
        }
        if (std::isdigit(File[Pos]) && !IsDBL)
        {
          Res *= 10;
          Res += File[Pos] - '0';
        }
        if (std::isdigit(File[Pos]) && IsDBL)
        {
          ResDBL += (File[Pos] - '0') * multDBl;
          multDBl /= 10;
        }
        if (!std::isdigit(File[Pos]) && (File[Pos] == 'e' || File[Pos] == 'E'))
        {
          Pos++;
          if (File[Pos] == '-')
          {
            Pos++;
            deg_sign = -1;
          }
          else if (File[Pos] == '+')
            Pos++;
          while (Pos < Flen && std::isdigit(File[Pos]))
          {
            deg *= 10;
            deg += File[Pos] - '0';
            Pos++;
          }
        }
        if (!std::isdigit(File[Pos]))
          break;
        Pos++;
      }
      INT mult10 = deg == 0 ? 1 : pow(10, deg_sign * deg);
      if (!IsDBL)
        return json{Res * sign * mult10};
      else
        return json{ResDBL * sign * mult10};
      */
    } /* End of 'ParseNumber' function */

    /* String parsing function.
     * ARGUMENTS: None.
     * RETURNS:
     *   (std::string) parsed string.
     */
    std::string ParseString( VOID )
    {
      std::string Res {};

      if (Pos < Flen && File[Pos] != '\"')
        return Res;
      Pos++;
      while (Pos < Flen)
      {
        if (File[Pos] == '\"')
        {
          Pos++;
          return Res;
        }
        if (File[Pos] == 'u' && File[Pos - 1] == '\\')
        {
          if (Pos + 3 < Flen)
          {
            std::string hex = File.substr(Pos + 1, 4);
            INT code = std::stoi(hex, nullptr, 16);
            if (code < 0x80)
              Res += static_cast<CHAR>(code);
          }
          else
            Pos = Flen;
        }
        switch (File[Pos])
        {
        case '"':
        case '\\':
          Res += File[Pos];
          break;
        case '\b':
          Res += '\b';
          break;
        case '\t':
          Res += '\t';
          break;
        case '\r':
          Res += '\r';
          break;
        case '\n':
          Res += '\n';
          break;
        case '\f':
          Res += '\f';
          break;
        default:
          Res += File[Pos];
          break;
        }
        Pos++;
      }
      return Res;
    } /* End of 'ParseString' function */

    /* Parse JSON array function.
     * ARGUMENTS: None.
     * RETURNS:
     *   (json) parsed array.
     */
    json ParseArray( VOID )
    {
      json_array Res = json_array{};

      if (Pos >= Flen)
        return Res;
      Pos++;
      while (Pos < Flen && File[Pos] != ']')
      {
        SkipWhiteSpace();
        SkipComment();
        if (File[Pos] == ']')
          break;
        Res.push_back(ParseValue());
        if (Pos < Flen && File[Pos] == ',')
          Pos++;
        SkipWhiteSpace();
      }
      if (Pos < Flen && File[Pos] == ']')
        Pos++;
      return Res;
    } /* End of 'ParseArray' function */

    /* Parse JSON object function.
     * ARGUMENTS: None.
     * RETURNS:
     *   (json) parsed object.
     */
    json ParseObject( VOID )
    {
      json_object Obj {};
      std::string Str {};
      json Current {};

      Pos++;
      while (Pos < Flen)
      {
        SkipWhiteSpace();
        if (Pos < Flen && File[Pos] == '}')
        {
          Pos++;
          return Obj;
        }
        if (Pos < Flen && File[Pos] == ',')
        {
          Pos++;
          continue;
        }
        SkipWhiteSpace();
        if (Pos < Flen && File[Pos] == '"')
          Str = ParseString();
        Pos++;
        SkipWhiteSpace();
        if (Pos < Flen && File[Pos] == ':')
        {
          Pos++;
        }
        SkipWhiteSpace();
        if (File[Pos] == '}')
          break;
        Current = ParseValue();
        Obj.insert({Str, Current});
        if (Pos < Flen && File[Pos] == '}')
        {
          Pos++;
          return Obj;
        }
        // Pos++;
      }
      return Obj;
    } /* End of 'ParseObject' function */

  public:
    /* Class default constructor */
    json_loader( VOID )
    {
    } /* End of 'json_loader' function */

    /* Class default constructor
     * ARGUMENTS:
     *   - file name:
     *       std::string FileName;
     */
    json_loader( std::string FileName )
    {
      std::ifstream f(FileName);

      File = std::string((std::istreambuf_iterator<char>(f)),
                          std::istreambuf_iterator<char>());
      f.close();
    } /* End of 'json_loader' function */

    /* Class default destructor.
     * ARGUMENTS: None.
     * RETURNS: None.
     */
    ~json_loader( VOID )
    {
    } /* End of '~json_loader' function */

    /* Load JSON from file function.
     * ARGUMENTS:
     *   - filename:
     *       const std::string FileName;
     *   - from start of reading to parse:
     *       INT Size = -1;
     *   - offset from start of file:
     *       INT Offset = 0;
     * RETURNS:
     *   (json) result JSON.
     */
    json Load( const std::string FileName = "", INT Size = -1, INT Offset = 0 )
    {
      if (FileName != "")
      {
        if (Offset != 0)
        {
          std::ifstream f(FileName, std::fstream::binary);
          File = std::string((std::istreambuf_iterator<char>(f)),
                              std::istreambuf_iterator<char>());
          f.close();
        }
        else
        {
          std::ifstream f(FileName);
          File = std::string((std::istreambuf_iterator<char>(f)),
                              std::istreambuf_iterator<char>());
          f.close();
        }
      }
      Flen = File.size();
      // Set offset and reading size based on arguments
      if (Offset < 0)
        Offset = 0;
      if (Size != -1)
        Flen = COM_MIN(Flen, Offset + Size);
      Pos = Offset;
      if (Pos > Flen)
        return {};
      if (Flen == 0)
        return {};
      if (File[Pos] == '[')
      {
        Pos++;
        json_array Res {};
        while (Pos < Flen && File[Pos] != ']')
        {
          SkipWhiteSpace();
          SkipComment();
          Res.push_back(ParseValue());
        }
        return Res;
      }
      else
      {
        Pos++;
        json_object Res {};
        SkipWhiteSpace();
        SkipComment();
        while (Pos < Flen && File[Pos] != '}')
        {
          std::string Name = ParseString();
          SkipWhiteSpace();
          if (Pos < Flen && File[Pos] == ':')
            Pos++;
          SkipWhiteSpace();
          json Data = ParseValue();
          Res[Name] = Data;
          Pos++;
          SkipWhiteSpace();
          SkipComment();
        }
        if (Pos < Flen)
          Pos++;
        return Res;
      }
    } /* End of 'Load' function */

  }; /* End of 'json_loader' class */
} /* end of 'tam' namespace */

#endif /* __tam_json_h_ */

/* END OF 'tam_json.h' FILE */
