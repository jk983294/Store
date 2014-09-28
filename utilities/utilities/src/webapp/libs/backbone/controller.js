/**
 * one shares the responsibility of a controller with both the Backbone.View and Backbone.Router
 */
var PhotoRouter = Backbone.Router.extend({
    routes: { "photos/:id": "route" },

    route: function( id ) {
        var item = photoCollection.get( id );
        var view = new PhotoView( { model: item } );

        $('.content').html( view.render().el );
    }
});
