# panels-wallpapers
Reversed-engineered wallpapers app on C, because we don't **love** paid AI-generated images.

**Inspired by mkbsd ;)**

## How to use

1. Open your terminal
2. ```gcc wallpapers.c -lcurl```
3. ```./a.out```
4. Enjoy these priceless wallpapers

## FAQ (Copy-paste from mkbsd)

### Q: What's the story behind this?

On September 24th, 2024, well-known tech YouTuber MKBHD released Panels, a wallpaper app that:

- Had insanely invasive, unjustified tracking including for location history and search history.
- Charged artists a predatory 50% commission (even Apple takes only 30% for app purchases).
- Forced you to watch two ads for every wallpaper that you wanted to download, and then only letting you download it in SD.
- Gatekept all HD wallpapers behind a **fifty dollars a year subscription**.
- Had many wallpapers that were essentially AI-generated slop or badly edited stock photos.

Especially given MKBHD's previous criticism of substandard companies and products, people justifiably got upset given that this looked like a pretty blatant grift and cash-grab that is exploitative of the fan base that's trusted his editorial integrity over the past fifteen years. However, on the same day, MKBHD wrote a post doubling down on the app.

### Q: Aren't you stealing from artists by running this script?

This program accesses publicly available media through the Panels app's own API. It doesn't do anything shady or illegal. The real problem here is Panels and MKBHD's complete inability to provide a secure platform for the artists that they're ~~exploiting~~ working with. Any other app could have avoided the issues that make this app possible had it been engineered competently.

