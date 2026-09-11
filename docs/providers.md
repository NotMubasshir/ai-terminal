# AI Providers

## Gemini

`GeminiProvider` uses the Gemini Interactions API over WinHTTP. It sends function declarations and consumes structured `function_call` steps. Tool results are returned as `function_result` steps using `previous_interaction_id`.

The API key comes from `GEMINI_API_KEY`.

## Mock

`MockProvider` is deterministic and intended for unit/integration testing. It requires no API key and never executes network requests.

## Provider boundary

The agent runtime depends only on `AIProvider`, `AIResponse`, `FunctionCall` and `FunctionResultInput`. Provider-specific HTTP, authentication and response parsing remain outside the runtime.
