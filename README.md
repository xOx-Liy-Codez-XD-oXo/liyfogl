# liyfogl
Library to render gc/wii fifo captures. it works barely enough to incorrectly render a very basic scene. My intent is to make it simple to use so you can just bolt it on a program as long as youve got a pointer to mem1 and mem2 and a pointer to a capture of every graphics command sent that frame

![Screenshot 2025-05-24 at 10 05 57 PM](https://github.com/user-attachments/assets/0b3df145-1d9f-49dc-aaf1-49efcbfbf98f)
### todo
* [ ] Fix whatever matrix bug is breaking Z compares
* [ ] Make the thing that turns a primitive command's data into separate arrays for the primitive drawer to use not use hardcoded offsets for the specific vertex description the test scene uses
* [ ] Add support for supplying a MEM1 dump for fetching indexed vertex data
* [ ] Figure out what indexed vertex color formats are
* [ ] Figure out what textures are. like what are they
