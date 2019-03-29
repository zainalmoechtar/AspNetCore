import { navigateTo, internalFunctions as uriHelperInternalFunctions } from './Services/UriHelper';
import { internalFunctions as httpInternalFunctions } from './Services/Http';
import { attachRootComponentToElement } from './Rendering/Renderer';

// Accept the following callbacks that need to be set before we boot.
const existing = window['Blazor'];

// Make the following APIs available in global scope for invocation from JS
window['Blazor'] = {
  navigateTo,

  callbacks: existing.callbacks,

  _internal: {
    attachRootComponentToElement,
    http: httpInternalFunctions,
    uriHelper: uriHelperInternalFunctions
  }
};
