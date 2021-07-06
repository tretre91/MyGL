/**
 * Sets the dark mode icon's image and the toggle button's title according to the
 * current dark mode state
 * @param {HTMLImageELement} icon The icon to change
 * @param {DoxygenAwesomeDarkModeToggle} button The button used to enable/disable dark mode
 */
function setIconState(icon, button) {
    if (DoxygenAwesomeDarkModeToggle.darkModeEnabled) {
        icon.src = "light_mode_white_24dp.svg";
        button.title = "Disable Dark Mode";
    } else {
        icon.src = "dark_mode_black_24dp.svg";
        button.title = "Enable Dark Mode";
    }
}

$(document).ready(function(){
    let darkModeIcon = document.createElement("img");
    darkModeIcon.setAttribute("id", "dark-mode-toggle-icon");

    let toggleButton = document.createElement("doxygen-awesome-dark-mode-toggle");
    toggleButton.appendChild(darkModeIcon);

    setIconState(darkModeIcon, toggleButton);

    document.addEventListener("click", event => {
        if (event.target.id == "dark-mode-toggle-icon") {
            setIconState(darkModeIcon, toggleButton);
        }
    });

    window.matchMedia("(prefers-color-scheme: dark)").addEventListener("change", _ => {
        setIconState(darkModeIcon, toggleButton);
    });

    document.getElementById("MSearchBox").parentNode.appendChild(toggleButton);
})