/***************************************************************
 * Copyright (C) 2026
 *    Computer Graphics Support Group of 30 Phys-Math Lyceum
 ***************************************************************/

/* FILE NAME   : tam_ai.cpp
 * PURPOSE     : Tough Arrangement Manager project.
 *               AI handle implementation file.
 * PROGRAMMER  : CGSG'Sr'2025.
 *               Arsentev Artemy (AA4).
 * LAST UPDATE : 01.04.2026
 * NOTE        : Namespace 'tam'.
 *
 * No part of this file may be changed without agreement of
 * Computer Graphics Support Group of 30 Phys-Math Lyceum
 */

#include "tam.h"

/* Build request for ai function.
 * ARGUMENTS:
 *   - Json file with room and furniture parametrs:
 *       const std::string &Input;
 * RETURNS:
 *   (std::string) Ready request for ai descriptions.
 */
std::string tam::ollama::BuildRequest( const std::string &Input ) 
{
  std::string Prompt =
    "You are an advanced spatial planning and interior layout optimization engine.                                                                           \n"

    "Your task is to analyze a JSON input describing a room and a set of furniture items, and generate an optimal furniture arrangement inside the room.     \n"

    "You MUST follow all rules strictly.                                                                                                                     \n"

    "========================                                                                                                                                \n"
    "INPUT DESCRIPTION                                                                                                                                       \n"
    "========================                                                                                                                                \n"
    "The input will be a JSON object containing:                                                                                                             \n"

    "1. Room properties:                                                                                                                                     \n"
    "- width (float)                                                                                                                                         \n"
    "- height (float)                                                                                                                                        \n"
    "- optional depth (for 3D rooms)                                                                                                                         \n"
    "- boundaries of the room (assume rectangular unless specified)                                                                                          \n"

    "2. Furniture items (array):                                                                                                                             \n"
    "Each furniture item contains:                                                                                                                           \n"
    "- id (string or integer)                                                                                                                                \n"
    "- type (string: bed, sofa, table, chair, cabinet, etc.)                                                                                                 \n"
    "- width (float)                                                                                                                                         \n"
    "- height (float)                                                                                                                                        \n"
    "- depth (float)                                                                                                                                         \n"
    "- rotation constraints (optional)                                                                                                                       \n"
    "- placement constraints (optional)                                                                                                                      \n"
    "- priority (optional)                                                                                                                                   \n"
    "                                                                                                                                                        \n"
    "========================                                                                                                                                \n"
    "GOALS                                                                                                                                                   \n"
    "========================                                                                                                                                \n"
    "You must place all furniture items in the room such that:                                                                                               \n"
    "                                                                                                                                                        \n"
    "1. No overlapping occurs between any objects.                                                                                                           \n"
    "2. All objects remain fully inside room boundaries.                                                                                                     \n"
    "3. Placement is physically plausible.                                                                                                                   \n"
    "4. Layout is aesthetically pleasing and realistic.                                                                                                      \n"
    "5. Circulation paths (walking space) are preserved.                                                                                                     \n"
    "6. Functional grouping is respected (e.g., sofa near table, bed with side access, etc.).                                                                \n"
    "7. Orientation of furniture is logical (e.g., bed head against a wall if possible).                                                                     \n"
    "                                                                                                                                                        \n"
    "========================                                                                                                                                \n"
    "OPTIMIZATION OBJECTIVES                                                                                                                                 \n"
    "========================                                                                                                                                \n"
    "You should optimize the layout according to:                                                                                                            \n"
    "                                                                                                                                                        \n"
    "- Minimize unused empty fragmented space                                                                                                                \n"
    "- Maximize usable walking areas                                                                                                                         \n"
    "- Maintain symmetry where appropriate                                                                                                                   \n"
    "- Align furniture with walls when reasonable                                                                                                            \n"
    "- Group related objects together                                                                                                                        \n"
    "- Avoid awkward rotations unless necessary                                                                                                              \n"
    "- Maintain accessibility for each object                                                                                                                \n"
    "                                                                                                                                                        \n"
    "========================                                                                                                                                \n"
    "COORDINATE SYSTEM                                                                                                                                       \n"
    "========================                                                                                                                                \n"
    "- The room origin (0,0) is the bottom-left corner.                                                                                                      \n"
    "- X axis goes to the right.                                                                                                                             \n"
    "- Y axis goes upward.                                                                                                                                   \n"
    "                                                                                                                                                        \n"
    "Each furniture item must include:                                                                                                                       \n"
    "                                                                                                                                                        \n"
    "- position:                                                                                                                                             \n"
    "    x (float)                                                                                                                                           \n"
    "    y (float)                                                                                                                                           \n"
                                                                                                                                                             
    "- rotation:                                                                                                                                             \n"
    "    angle in degrees (0, 90, 180, 270 typically)                                                                                                        \n"
    "                                                                                                                                                        \n"
    "- optionally:                                                                                                                                           \n"
    "    bounding box after rotation                                                                                                                         \n"
    "                                                                                                                                                        \n"
    "========================                                                                                                                                \n"
    "OUTPUT FORMAT (STRICT)                                                                                                                                  \n"
    "========================                                                                                                                                \n"
    "You MUST output ONLY valid JSON.                                                                                                                        \n"
    "                                                                                                                                                        \n"
    "Do NOT include:                                                                                                                                         \n"
    "- explanations                                                                                                                                          \n"
    "- comments                                                                                                                                              \n"
    "- markdown                                                                                                                                              \n"
    "- additional text                                                                                                                                       \n"
    "                                                                                                                                                        \n"
    "Output schema:                                                                                                                                          \n"
    "                                                                                                                                                        \n"
    "{                                                                                                                                                       \n"
    "  \"layout\": [                                                                                                                                           \n"
    "    {                                                                                                                                                   \n"
    "      \"id\": \"...\",                                                                                                                                      \n"
    "      \"type\": \"...\",                                                                                                                                    \n"
    "      \"x\": float,                                                                                                                                       \n"
    "      \"y\": float,                                                                                                                                       \n"
    "      \"rotation\": float,                                                                                                                                \n"
    "      \"width\": float,                                                                                                                                   \n"
    "      \"height\": float,                                                                                                                                  \n"
    "      \"depth\": float                                                                                                                                    \n"
    "    }                                                                                                                                                   \n"
    "  ]                                                                                                                                                     \n"
    "}                                                                                                                                                       \n"
    "                                                                                                                                                        \n"
    "========================                                                                                                                                \n"
    "STRICT RULES                                                                                                                                            \n"
    "========================                                                                                                                                \n"
    "- Output ONLY JSON                                                                                                                                      \n"
    "- No extra text before or after JSON                                                                                                                    \n"
    "- No comments in JSON                                                                                                                                   \n"
    "- All numbers must be valid floats                                                                                                                      \n"
    "- Do not omit any furniture item                                                                                                                        \n"
    "- Do not invent new furniture items                                                                                                                     \n"
    "- Do not change IDs                                                                                                                                     \n"
    "- Do not remove or duplicate objects                                                                                                                    \n"
    "                                                                                                                                                        \n"
    "========================                                                                                                                                \n"
    "VALIDATION RULES                                                                                                                                        \n"
    "========================                                                                                                                                \n"
    "Ensure that:                                                                                                                                            \n"
    "- All objects are inside room bounds                                                                                                                    \n"
    "- No intersections between bounding boxes                                                                                                               \n"
    "- All objects have valid numeric coordinates                                                                                                            \n"
    "- Rotations are consistent with object dimensions                                                                                                       \n"
    "- Layout is stable and realistic                                                                                                                        \n"
    "                                                                                                                                                        \n"
    "========================                                                                                                                                \n"
    "INPUT                                                                                                                                                   \n"
    "========================                                                                                                                                \n"
    "                                                                                                                                                        \n"
    "                                                                                                                                                        \n"
    "You are a spatial layout engine.\n"                                                                                                                     
    "Place furniture in the room.\n"                                                                                                                         
    "Rules:\n"                                                                                                                                               
    "- No overlaps\n"                                                                                                                                        
    "- Stay inside bounds\n"                                                                                                                                 
    "- Output ONLY JSON\n"                                                                                                                                   
    "- Do not write anything else\n"                                                                                                                         
    "Input:\n" + Input;                                                                                                                                      
                                                                                                                                                             
  return std::string("{") +                                                                                                                                  
    "\"model\":\"llama3\"," +                                                                                                                
    "\"prompt\":\"" + ConvertJson(Prompt) + "\"," +                                                                                                          
    "\"stream\":false" +                                                                                                                                     
    "}";                                                                                                                                                     
} /* End of 'tam::ollama::BuildRequest' function */                                                                                                          
                                                                                                                                                             
/* Send ready request to ai function.                                                                                                                        
 * ARGUMENTS:
 *   - Ready request for ai:
 *       const std::string &Request;
 * RETURNS:
 *   (std::string) ai response.
 */
std::string tam::ollama::SendRequest( const std::string &Request ) 
{
  // Open session
  HINTERNET hSession = WinHttpOpen(L"OllamaClient/1.0", WINHTTP_ACCESS_TYPE_DEFAULT_PROXY,
    WINHTTP_NO_PROXY_NAME, WINHTTP_NO_PROXY_BYPASS, 0);
  if (!hSession) 
    return "";

  // Connect session
  HINTERNET hConnect = WinHttpConnect(hSession, Host.c_str(), Port, 0);
  if (!hConnect) 
    return "";

  // API request params
  HINTERNET hRequest = WinHttpOpenRequest(hConnect, L"POST", L"/api/generate", nullptr, WINHTTP_NO_REFERER, WINHTTP_DEFAULT_ACCEPT_TYPES, 0);

  // Get JSON data headers
  std::wstring headers =
    L"Content-Type: application/json\r\n"
    L"Accept: application/json\r\n";

  // Send request
  BOOL result = WinHttpSendRequest(hRequest, headers.c_str(), -1,
    (LPVOID)Request.c_str(), (DWORD)Request.size(), (DWORD)Request.size(), 0);

  if (!result) 
    return "";
  
  // Receive response
  WinHttpReceiveResponse(hRequest, nullptr);

  // Read response
  std::string response;
  DWORD size = 0;

  do 
  {
    DWORD downloaded = 0;
    WinHttpQueryDataAvailable(hRequest, &size);

    if (size == 0) 
      break;

    std::vector<CHAR> buffer((INT)size + 1);   

    WinHttpReadData(hRequest, buffer.data(), size, &downloaded);
    response.append(buffer.data(), downloaded);
  } while (size > 0);

  // Close session
  WinHttpCloseHandle(hRequest);
  WinHttpCloseHandle(hConnect);
  WinHttpCloseHandle(hSession);

  return response;
} /* End of 'tam::ollama::SendRequest' function */

/* Parse ai response function.
 * ARGUMENTS:
 *   - Ai response:
 *       const std::string &Response;
 * RETURNS:
 *   (std::string) parsed response.
 */
std::string tam::ollama::ParseResponse( const std::string &Response )
{
  std::string key = "\"response\":\"";
  size_t start = Response.find(key);
  //if (start == std::string::npos) 
  //  return "";

  start += key.length();

  std::string result;
  BOOL escape = FALSE;

  for (SIZE_T i = start; i < Response.size(); i++) 
  {
    CHAR c = Response[i];

    if (escape) 
    {
      result += c;
      escape = FALSE;
      continue;
    }

    if (c == '\\') 
    {
      escape = TRUE;
      continue;
    }

    if (c == '"') 
      break;

    result += c;
  }

  return result;
} /* End of 'tam::ollama::ParseResponse' function */

/* Convert ai response to JSON function.
 * ARGUMENTS:
 *   - Ai response:
 *       const std::string &Response;
 * RETURNS:
 *   (std::string) converted JSON.
 */
std::string tam::ollama::ConvertResponse(const std::string &Response)
{
  std::string key = "\"response\":\"";
  SIZE_T start = Response.find(key);

  if (start == std::string::npos)
    return "";

  start += key.length();

  std::string result;
  BOOL escape = FALSE;

  for (SIZE_T i = start; i < Response.size(); i++)
  {
    CHAR c = Response[i];

    if (escape)
    {
      switch (c)
      {
      case 'n': 
        result += '\n'; 
        break;
      case 't':
        result += '\t'; 
        break;
      case '"': 
        result += '"'; 
        break;
      case '\\': 
        result += '\\'; 
        break;
      default: 
        result += c;
        break;
      }
      escape = FALSE;
      continue;
    }

    if (c == '\\')
    {
      escape = TRUE;
      continue;
    }

    if (c == '"')
      break;

    if (static_cast<UCHAR>(c) > 127)
      continue;

    result += c;
  }

  return result;
} /* End of 'tam::ollama::ConvertResponse' function */

/* Convert JSON to spesific request typr for ai function.
 * ARGUMENTS:
 *   - JSON input:
 *       const std::string &Input;
 * RETURNS:
 *   (std::string) parsed response.
 */
std::string tam::ollama::ConvertJson( const std::string &Input )
{
  std::string output;
  for (char c : Input) 
  {
    switch (c) 
    {
    case '"': 
      output += "\\\""; 
      break;
    case '\\': 
      output += "\\\\"; 
      break;
    case '\n': 
      output += "\\n";
      break;
    case '\r':
      break;
    default:
      output += c; 
      break;
    }
  }
  return output;
} /* End of 'tam::ollama::ConvertJson' function */

/* Generate layout with ai function.
 * ARGUMENTS:
 *   - Json file with room and furniture parametrs:
 *       const std::string &Input;
 * RETURNS:
 *   (std::string) Json file with arranged furniture descriptions.
 */
std::string tam::ollama::GenerateLayout( const std::string &Input )
{
  std::string Request = BuildRequest(Input);
  std::string Response = SendRequest(Request);

  return ConvertResponse(Response);
} /* End of 'tam::ollama::GenerateLayout' function */

std::string tam::ollama::BuildTextRequest( const std::string &Request ) 
{
  //helpful interior designer
  std::string Prompt =
    "You are very clever, friendly, helpful assistant.\n"
    "Answer clearly, precisely, without unnecessary information, what they are asking for.\n"
    "If the question requires detailed explanations, please answer in detail with all clarifications.\n"
    "Never write any Chinese symbols\n"
    "Explain clearly and concisely:\n"
    + Request;

  return std::string("{") +
    "\"model\":\"deepseek-v3.1:671b-cloud\"," +
    "\"prompt\":\"" + ConvertJson(Prompt) + "\"," +
    "\"stream\":false" +
    "}";
} /* End of 'tam::ollama::BuildTextRequest' function */

/* Format text function.
 * ARGUMENTS:
 *   - Text to format:
 *       const std::string &Text;
 * RETURNS:
 *   (std::string) Formated text.
 */
std::string tam::ollama::FormatText(const std::string &Text ) 
{
  const size_t MAX_LINE = 50;

  std::string result;
  size_t lineLength = 0;

  for (size_t i = 0; i < Text.size(); i++) 
  {
    CHAR c = Text[i];

    if (c == '\n') 
    {
      result += c;
      lineLength = 0;
      continue;
    }

    if (lineLength >= MAX_LINE) 
    {
      if (c == ' ') 
      {
        result += '\n';
        lineLength = 0;
        continue;
      }
      else 
      {
        SIZE_T back = result.size();
        while (back > 0 && result[back - 1] != ' ' && result[back - 1] != '\n')
          back--;

        if (back > 0) 
        {
          result.insert(back, "\n");
          lineLength = result.size() - back;
        }
      }
    }

    result += c;
    lineLength++;

    if (c == '.' || c == '!' || c == '?') 
    {
      if (i > 0 && i + 1 < Text.size()) 
      {
        if (isdigit(Text[i - 1]) && isdigit(Text[i + 1])) 
          continue;
      }

      result += '\n';
      lineLength = 0;
    }
  }

  return result;
} /* End of 'tam::ollama::FormatText' function */

/* Generate answer with ai function.
 * ARGUMENTS:
 *   - Input request:
 *       const std::string &Input;
 * RETURNS:
 *   (std::string) response string.
 */
std::string tam::ollama::GenerateAnswer( const std::string &Input )
{
  std::string Request = BuildTextRequest(Input);
  std::string Response = SendRequest(Request);
  std::string text = ConvertResponse(Response);

  return FormatText(text);
} /* End of 'tam::ollama::GenerateAnswer' function */

/* Start Ollama process function.
 * ARGUMENTS: None.
 * RETURNS:
 *   (BOOL) Is Ollama started.
 */
BOOL tam::ollama::StartOllamaProcess( VOID )
{
  STARTUPINFOW si {};
  si.cb = sizeof(si);

  ZeroMemory(&OllamaProcess, sizeof(OllamaProcess));

  std::wstring cmd = L"ollama serve";

  BOOL ok = CreateProcessW(nullptr, cmd.data(), nullptr, nullptr, FALSE,
    CREATE_NO_WINDOW, nullptr, nullptr, &si, &OllamaProcess);

  if (!ok)
    return FALSE;

  OllamaStartedByUs = TRUE;
  return TRUE;
} /* End of 'tam::ollama::StartOllamaProcess' function */

/* Pull model in cmd function.
 * ARGUMENTS:
 *   - Model name:
 *       const std::string &Model;
 * RETURNS:
 *   (BOOL) If pulled succesfully.
 */
BOOL tam::ollama::PullModelCMD( const std::wstring &Model )
{
  std::wstring cmd = L"cmd /C ollama pull " + Model;

  STARTUPINFOW si = {sizeof(si)};
  PROCESS_INFORMATION pi;

  BOOL ok = CreateProcessW(nullptr, cmd.data(), nullptr, nullptr, FALSE,
    0, nullptr, nullptr, &si, &pi);

  if (!ok)
    return FALSE;

  // wait for end of loading
  WaitForSingleObject(pi.hProcess, INFINITE);

  CloseHandle(pi.hProcess);
  CloseHandle(pi.hThread);

  return TRUE;
} /* End of 'tam::ollama::PullModelCMD' function */

/* Check if all ai parts are ready function.
 * ARGUMENTS:
 *   - Model name:
 *       const std::string &Model;
 * RETURNS:
 *   (BOOL) If ai is ready to use.
 */
BOOL tam::ollama::EnsureAll( const std::string &Model )
{
  if (!IsOllamaRunning())
  {
    tam::logger::out << "Starting Ollama...\n";

    if (!StartOllamaProcess())
      return FALSE;
    //Sleep(3000);
  }

  if (!HasModel(Model))
  {
    tam::logger::out << "Pulling model...\n";

    if (!PullModelCMD(std::wstring(Model.begin(), Model.end())))
      return FALSE;
  }

  return TRUE;
} /* End of 'tam::ollama::EnsureAll' function */

/* Check if Ollama is running function.
 * ARGUMENTS: None.
 * RETURNS:
 *   (BOOL) If Ollama is running.
 */
BOOL tam::ollama::IsOllamaRunning( VOID )
{
  std::string response = HttpGet(Host, L"/api/tags");

  if (response.empty())
    return FALSE;

  // base check if it's JSON Ollama
  if (response.find("\"models\"") != std::string::npos)
    return TRUE;

  return FALSE;
} /* End of 'tam::ollama::IsOllamaRunning' function */

/* Check if Ollama has current model function.
 * ARGUMENTS:
 *   - Model name:
 *       const std::string &Model;
 * RETURNS:
 *   (BOOL) If Ollama has current model.
 */
BOOL tam::ollama::HasModel( const std::string &Model )
{
  std::string response = HttpGet(Host, L"/api/tags");

  if (response.empty())
    return FALSE;

  // Find model by name
  if (response.find(Model) != std::string::npos)
    return TRUE;

  return FALSE;
} /* End of 'tam::ollama::HasModel' function */

/* Make a http GET request function.
 * ARGUMENTS:
 *   - Host to send a request:
 *       const std::wstring &Host;
 *   - API tags:
 *       const std::string &Path;
 * RETURNS:
 *   (std::string) raw JSON.
 */
std::string tam::ollama::HttpGet( const std::wstring &Host, const std::wstring &Path )
{
  std::string result;

  HINTERNET hSession = WinHttpOpen(L"OllamaClient/1.0", WINHTTP_ACCESS_TYPE_DEFAULT_PROXY,
    WINHTTP_NO_PROXY_NAME, WINHTTP_NO_PROXY_BYPASS, 0);

  if (!hSession)
    return "";

  HINTERNET hConnect = WinHttpConnect(hSession, Host.c_str(), Port, 0);

  if (!hConnect)
  {
    WinHttpCloseHandle(hSession);
    return "";
  }

  HINTERNET hRequest = WinHttpOpenRequest(hConnect, L"GET", Path.c_str(),
    nullptr, WINHTTP_NO_REFERER, WINHTTP_DEFAULT_ACCEPT_TYPES, 0);

  if (!hRequest)
  {
    WinHttpCloseHandle(hConnect);
    WinHttpCloseHandle(hSession);
    return "";
  }

  std::wstring headers = L"Accept: application/json\r\n";

  BOOL ok = WinHttpSendRequest(hRequest, headers.c_str(), -1,
    WINHTTP_NO_REQUEST_DATA, 0, 0, 0);

  if (!ok)
  {
    WinHttpCloseHandle(hRequest);
    WinHttpCloseHandle(hConnect);
    WinHttpCloseHandle(hSession);
    return "";
  }

  if (!WinHttpReceiveResponse(hRequest, nullptr))
  {
    WinHttpCloseHandle(hRequest);
    WinHttpCloseHandle(hConnect);
    WinHttpCloseHandle(hSession);
    return "";
  }

  DWORD size = 0;

  do
  {
    DWORD downloaded = 0;

    if (!WinHttpQueryDataAvailable(hRequest, &size) || size == 0)
      break;

    std::vector<CHAR> buffer(size);

    if (!WinHttpReadData(hRequest, buffer.data(), size, &downloaded))
      break;

    result.append(buffer.data(), downloaded);

  } while (size > 0);

  WinHttpCloseHandle(hRequest);
  WinHttpCloseHandle(hConnect);
  WinHttpCloseHandle(hSession);

  return result;
} /* End of 'tam::ollama::HttpGet' function */

/* Stop Ollama process function.
 * ARGUMENTS: None.
 * RETURNS:
 *   (BOOL) If Ollama is stopped.
 */
BOOL tam::ollama::StopOllama( VOID )
{
  if (!OllamaStartedByUs)
    return TRUE; // skip not our process 

  if (OllamaProcess.hProcess == nullptr)
    return FALSE;

  // check if process is alive
  DWORD exitCode = 0;
  if (!GetExitCodeProcess(OllamaProcess.hProcess, &exitCode))
    return FALSE;

  if (exitCode == STILL_ACTIVE)
  {
    // terminate process
    TerminateProcess(OllamaProcess.hProcess, 0);

    WaitForSingleObject(OllamaProcess.hProcess, 3000);
  }

  CloseHandle(OllamaProcess.hProcess);
  CloseHandle(OllamaProcess.hThread);

  OllamaProcess.hProcess = nullptr;
  OllamaProcess.hThread = nullptr;

  return TRUE;
} /* End of 'tam::ollama::StopOllama' function */

/* END OF 'tam_ai.cpp' FILE */
